#pragma once

#include "core/point.hpp"
#include "framework/system.hpp"
#include <entt.hpp>
#include <functional>

namespace systems::player {

class Defeat : public framework::System {
  private:
    static constexpr unsigned int DEFEAT_PAUSE_TICKS = 100;

    unsigned int defeat_pause_counter = 0;
    bool defeat_pause_ongoing = false;
    bool game_is_over = false;
    std::function<void(entt::registry &, core::Point, unsigned int)> add_explosion;
    std::function<void(entt::registry &)> add_player;
    std::function<void(entt::registry &)> add_fortresses;

    void handle_defeat_if_any(framework::ExecuteCtx &ctx);
    void handle_defeat(framework::ExecuteCtx &ctx, entt::entity player_entity);
    void handle_ongoing_pause(framework::ExecuteCtx &ctx);
    void delete_all_entities(framework::ExecuteCtx &ctx);

  public:
    Defeat(
        std::function<void(entt::registry &, core::Point, unsigned int)> add_explosion,
        std::function<void(entt::registry &)> add_player,
        std::function<void(entt::registry &)> add_fortresses
    );
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems::player