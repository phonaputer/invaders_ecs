#pragma once

#include "framework/event_broker.hpp"
#include "framework/player_input_manager.hpp"
#include "framework/renderer.hpp"
#include "framework/scene.hpp"
#include "framework/sdl_asset_manager.hpp"
#include "framework/sdl_renderer.hpp"
#include "framework/system.hpp"
#include "framework/system_registry.hpp"
#include <SDL3/SDL.h>
#include <entt.hpp>
#include <memory>
#include <optional>

namespace framework {

class Game : public SystemRegistry {
  private:
    // Window and Renderer are owned and cleaned up by SDL.
    SDL_Window *window;
    SDL_Renderer *renderer;

    std::unique_ptr<Renderer> renderer_wrapper;
    std::unique_ptr<PlayerInputManager> player_input_manager;
    std::shared_ptr<SDLAssetManager> asset_manager;
    EventBroker event_broker;
    entt::registry ecs;

    std::vector<std::unique_ptr<System>> update_systems;
    std::vector<std::unique_ptr<System>> draw_systems;

    bool have_active_scene = false;

    Uint64 previous_now_ms;
    Uint64 unprocessed_ms;

  public:
    Game();
    void set_scene(std::unique_ptr<Scene> scene);
    void update();
    void draw();
    PlayerInputManager &get_player_input_manager();

    void add_update_system(std::unique_ptr<System> system) override;
    void add_draw_system(std::unique_ptr<System> system) override;
};

} // namespace framework