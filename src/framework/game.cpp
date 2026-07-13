#include "framework/game.hpp"
#include "framework/constants.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/default_ecs.hpp"
#include "framework/ecs/event_broker.hpp"
#include "framework/player_input_manager.hpp"
#include "framework/scene.hpp"
#include "framework/scene_initialization_context.hpp"
#include "framework/sdl_asset_manager.hpp"
#include "framework/sdl_renderer.hpp"
#include <SDL3/SDL.h>
#include <format>
#include <memory>
#include <optional>
#include <stdexcept>

namespace framework {

const Uint64 TARGET_FPS = 60;
const Uint64 MS_PER_UPDATE = 1000 / TARGET_FPS;

// FIXME - tweak this for web output
const int ACTUAL_WINDOW_WIDTH = 672;
const int ACTUAL_WINDOW_HEIGHT = 864;

Game::Game() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    throw std::runtime_error(std::format("Failed to initialize SDL: {}", SDL_GetError()));
  }

  window = SDL_CreateWindow(
      "Gallia", ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_HEIGHT, SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE
  );
  if (!window) {
    throw std::runtime_error(std::format("Couldn't create window: {}", SDL_GetError()));
  }

  renderer = SDL_CreateRenderer(window, NULL);
  if (!renderer) {
    throw std::runtime_error(std::format("Couldn't create renderer: {}", SDL_GetError()));
  }

  SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

  previous_now_ms = SDL_GetTicks();
  unprocessed_ms = 0;

  player_input_manager = std::make_unique<PlayerInputManager>();
  asset_manager = std::make_shared<SDLAssetManager>(renderer);
  renderer_wrapper = std::make_unique<SDLRenderer>(renderer, asset_manager);
}

void Game::update() {
  if (!have_active_scene) {
    return;
  }

  const auto now_ms = SDL_GetTicks();
  unprocessed_ms += now_ms - previous_now_ms;
  previous_now_ms = now_ms;

  while (unprocessed_ms > MS_PER_UPDATE) {
    player_input_manager->update();

    world.progress();

    unprocessed_ms -= MS_PER_UPDATE;
  }

  // Doing this should save some CPU by preventing SDL from executing this
  // function as fast as possible.
  // A cursory test seemed to indicate this cuts CPU usage by about 50%.
  const Uint64 frameTime = SDL_GetTicks() - now_ms;
  if (MS_PER_UPDATE > frameTime) {
    SDL_Delay(MS_PER_UPDATE - frameTime);
  }
}

void Game::draw() {
  if (!have_active_scene) {
    return;
  }

  SDL_SetRenderDrawColorFloat(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);
  SDL_RenderClear(renderer);

  for (const auto &system : draw_systems) {
    system.run();
  }

  SDL_RenderPresent(renderer);
}

void Game::set_scene(std::unique_ptr<Scene> scene) {
  scene->initialize(
      SceneInitializationContext{
          .assets = *asset_manager,
          .world = world,
          .renderer = *renderer_wrapper,
          .player_input_manager = *player_input_manager,
      }
  );

  have_active_scene = true;
};

PlayerInputManager &Game::get_player_input_manager() {
  return *player_input_manager;
}

void Game::add_draw_system(flecs::system system) {
  // This prevents the system from running in world.progress().
  // Would be nice enough to have this run as part of world.progress(), but I
  // don't want to refactor the entire game to use deltaT instead of a fixed
  // frame rate.
  system.disable();

  draw_systems.push_back(system);
}

} // namespace framework