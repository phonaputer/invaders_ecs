#pragma once

#include "framework/system.hpp"
#include <entt.hpp>
#include <functional>

namespace systems::player {

class GameOver : public framework::System {
  private:
    static constexpr unsigned int PAUSE_TICKS = 170;

    unsigned int pause_counter = 0;
    bool pause_ongoing = false;
    bool game_is_over = false;
    std::function<void(entt::registry &, float, float, unsigned int)> add_explosion;
    std::function<void(entt::registry &)> add_player;
    std::function<void(entt::registry &)> add_fortresses;

    void handle_defeat_if_any(framework::ExecuteCtx &ctx);
    void handle_alien_landing(framework::ExecuteCtx &ctx);
    void handle_player_defeat(framework::ExecuteCtx &ctx, entt::entity player_entity);
    void handle_ongoing_pause(framework::ExecuteCtx &ctx);
    void delete_all_entities(framework::ExecuteCtx &ctx);

  public:
    GameOver(
        std::function<void(entt::registry &, float, float, unsigned int)> add_explosion,
        std::function<void(entt::registry &)> add_player,
        std::function<void(entt::registry &)> add_fortresses
    );
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems::player