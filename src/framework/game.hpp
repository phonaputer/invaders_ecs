#pragma once

#include "framework/ecs/ecs.hpp"
#include "framework/player_input_manager.hpp"
#include "framework/renderer.hpp"
#include "framework/scene.hpp"
#include "framework/sdl_asset_manager.hpp"
#include "framework/sdl_renderer.hpp"
#include <SDL3/SDL.h>
#include <flecs.h> // not used for anything yet, just checking if this can compile
#include <memory>
#include <optional>

namespace framework {

class Game {
  private:
    // Window and Renderer are owned and cleaned up by SDL.
    SDL_Window *window;
    SDL_Renderer *renderer;

    std::unique_ptr<Renderer> renderer_wrapper;
    std::unique_ptr<PlayerInputManager> player_input_manager;
    std::shared_ptr<SDLAssetManager> asset_manager;
    std::unique_ptr<ecs::ECS> ecs;

    bool have_active_scene = false;

    Uint64 previous_now_ms;
    Uint64 unprocessed_ms;

  public:
    Game();
    void set_scene(std::unique_ptr<Scene> scene);
    void update();
    void draw();
    PlayerInputManager &get_player_input_manager();
};

} // namespace framework