#define SDL_MAIN_USE_CALLBACKS

#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/default_ecs.hpp"
#include "framework/ecs/system.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <memory>

// TODO
// * Test ECS and see if it works.
// * Maybe add some unit tests.
// * Add a player spaceship.
// * Make player spaceship movable with the arrow keys.
// * Allow player spaceship to shoot projectiles with spacebar.
SDL_AppResult SDL_AppInit([[maybe_unused]] void **appstate, [[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  SDL_Log("Setup complete...");

  // just try it
  std::vector<std::unique_ptr<ecs::System>> systems;
  auto component_manager = std::make_unique<ecs::ComponentManager>();
  auto ecs = std::make_unique<ecs::DefaultECS>(std::move(systems), std::move(component_manager));

  // TODO actually test this thing

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent([[maybe_unused]] void *appstate, [[maybe_unused]] SDL_Event *event) {
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate([[maybe_unused]] void *appstate) {
  return SDL_APP_SUCCESS;
}

void SDL_AppQuit([[maybe_unused]] void *appstate, [[maybe_unused]] SDL_AppResult result) {
  SDL_Log("Quitting...");
}
