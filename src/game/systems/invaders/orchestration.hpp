#pragma once

#include "core/point.hpp"
#include "framework/constants.hpp"
#include "framework/system.hpp"
#include <entt.hpp>
#include <functional>
#include <random>

namespace systems::invaders {

class Orchestration : public framework::System {
  private:
    static constexpr int BASE_TICKS_PER_MOVE = 58;
    static constexpr float X_SPEED = 5;
    static constexpr float Y_SPEED = 5;
    static constexpr float LEFT_MOVEMENT_BOUNDARY = 0;
    static constexpr float RIGHT_MOVEMENT_BOUNDARY = framework::WINDOW_WIDTH;
    static constexpr int TICKS_PER_SHOOT_CHANCE = 12;
    static constexpr int ALIEN_SHOOT_CHANCE = 3;

    bool paused = false;

    bool move_right = true;
    int tick_counter = 0;
    std::function<void(entt::registry &, core::Point)> add_projectile;
    std::function<void(entt::registry &)> rerack_aliens;
    std::mt19937 rand_gen;
    unsigned int shoot_counter = 0;

    bool should_shoot_this_tick();
    void random_alien_shoot(entt::registry &ecs);
    bool should_move_this_tick();
    bool did_hit_wall(entt::registry &ecs);
    void move(entt::registry &ecs, bool move_down);
    void animate(entt::registry &ecs);

  public:
    Orchestration(
        std::function<void(entt::registry &, core::Point)> add_projectile,
        std::function<void(entt::registry &)> rerack_aliens,
        unsigned int rand_seed
    );
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems::invaders