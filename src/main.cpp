#define SDL_MAIN_USE_CALLBACKS

#include "framework/game/game.hpp"
#include "framework/game/player_input.hpp"
#include "gallia/test_scene.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <memory>

// TODO
// * Test ECS and see if it works.
// * Maybe add some unit tests.
// * Add a player spaceship.
// * Make player spaceship movable with the arrow keys.
// * Allow player spaceship to shoot projectiles with spacebar.
SDL_AppResult SDL_AppInit(void **appstate, [[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  game::Game *game = nullptr;
  try {
    game = new game::Game();
    *appstate = game;
  } catch (const std::exception &e) {
    return SDL_APP_FAILURE;
  }

  auto test_scene = std::make_unique<gallia::TestScene>();
  game->set_scene(std::move(test_scene));

  SDL_Log("Setup complete...");

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, [[maybe_unused]] SDL_Event *event) {
  auto game = (game::Game *)appstate;

  switch (event->type) {
    case SDL_EVENT_QUIT:
      return SDL_APP_SUCCESS;

    case SDL_EVENT_KEY_DOWN:
      switch (event->key.scancode) {
        case SDL_SCANCODE_A:
          game->get_player_input_manager().engage(game::PlayerInput::LEFT);
          break;
        case SDL_SCANCODE_D:
          game->get_player_input_manager().engage(game::PlayerInput::RIGHT);
          break;
        case SDL_SCANCODE_SPACE:
          game->get_player_input_manager().engage(game::PlayerInput::FIRE);
          break;
        case SDL_SCANCODE_W:
          game->get_player_input_manager().engage(game::PlayerInput::UP);
          break;
        case SDL_SCANCODE_S:
          game->get_player_input_manager().engage(game::PlayerInput::DOWN);
          break;
        case SDL_SCANCODE_ESCAPE:
          game->get_player_input_manager().engage(game::PlayerInput::PAUSE);
          break;
        default:
          // do nothing
      }
      break;

    case SDL_EVENT_KEY_UP:
      switch (event->key.scancode) {
        case SDL_SCANCODE_A:
          game->get_player_input_manager().disengage(game::PlayerInput::LEFT);
          break;
        case SDL_SCANCODE_D:
          game->get_player_input_manager().disengage(game::PlayerInput::RIGHT);
          break;
        case SDL_SCANCODE_SPACE:
          game->get_player_input_manager().disengage(game::PlayerInput::FIRE);
          break;
        case SDL_SCANCODE_W:
          game->get_player_input_manager().disengage(game::PlayerInput::UP);
          break;
        case SDL_SCANCODE_S:
          game->get_player_input_manager().disengage(game::PlayerInput::DOWN);
          break;
        case SDL_SCANCODE_ESCAPE:
          game->get_player_input_manager().disengage(game::PlayerInput::PAUSE);
          break;
        default:
          // do nothing
      }
      break;
  }

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  auto game = (game::Game *)appstate;

  game->update();
  game->draw();

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, [[maybe_unused]] SDL_AppResult result) {
  auto game = (game::Game *)appstate;

  delete game;

  SDL_Log("Quitting...");
}
