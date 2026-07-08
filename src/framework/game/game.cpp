#include "framework/game/game.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/default_ecs.hpp"
#include "framework/ecs/message_board.hpp"
#include "framework/game/constants.hpp"
#include "framework/game/player_input_manager.hpp"
#include "framework/game/scene.hpp"
#include "framework/game/scene_initialization_context.hpp"
#include "framework/game/sdl_asset_manager.hpp"
#include "framework/game/sdl_renderer.hpp"
#include <SDL3/SDL.h>
#include <format>
#include <memory>
#include <optional>
#include <stdexcept>

namespace game {

// Roughly 60 updates per second. 1000 / 60 = 16.66 (repeating, of course).
const Uint64 MS_PER_UPDATE = 17;

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

    ecs->update();

    unprocessed_ms -= MS_PER_UPDATE;
  }
}

void Game::draw() {
  if (!have_active_scene) {
    return;
  }

  SDL_SetRenderDrawColorFloat(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);
  SDL_RenderClear(renderer);

  ecs->draw();

  SDL_RenderPresent(renderer);
}

void Game::set_scene(std::unique_ptr<Scene> scene) {
  ecs = std::make_unique<ecs::DefaultECS>(
      std::make_unique<ecs::ComponentManager>(), std::make_unique<ecs::MessageBoard>()
  );

  scene->initialize(
      SceneInitializationContext{
          .assets = *asset_manager,
          .ecs = *ecs,
          .renderer = *renderer_wrapper,
          .player_input_manager = *player_input_manager,
      }
  );

  have_active_scene = true;
};

PlayerInputManager &Game::get_player_input_manager() {
  return *player_input_manager;
}

} // namespace game