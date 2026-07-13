#pragma once

#include "framework/ecs/ecs.hpp"
#include "framework/player_input_manager.hpp"
#include "framework/renderer.hpp"
#include "framework/scene.hpp"
#include "framework/sdl_asset_manager.hpp"
#include "framework/sdl_renderer.hpp"
#include "framework/system_adder.hpp"
#include <SDL3/SDL.h>
#include <flecs.h>
#include <memory>
#include <optional>
#include <vector>

namespace framework {

class Game : public SystemAdder {
  private:
    // Window and Renderer are owned and cleaned up by SDL.
    SDL_Window *window;
    SDL_Renderer *renderer;

    flecs::world world;
    std::unique_ptr<Renderer> renderer_wrapper;
    std::unique_ptr<PlayerInputManager> player_input_manager;
    std::shared_ptr<SDLAssetManager> asset_manager;
    std::vector<flecs::system> draw_systems;

    bool have_active_scene = false;

    Uint64 previous_now_ms;
    Uint64 unprocessed_ms;

  public:
    Game();
    void set_scene(std::unique_ptr<Scene> scene);
    void update();
    void draw();
    PlayerInputManager &get_player_input_manager();

    void add_draw_system(flecs::system system) override;
};

} // namespace framework