#pragma once

#include "framework/constants.hpp"
#include "framework/system.hpp"
#include "game/assets/asset_enums.hpp"
#include <array>
#include <entt.hpp>
#include <functional>
#include <random>

namespace systems::invaders {

class Orchestration : public framework::System {
  private:
    static constexpr int BASE_TICKS_PER_MOVE = 4;
    static constexpr float X_SPEED = 8;
    static constexpr float Y_SPEED = 12;
    static constexpr float LAST_MAN_STANDING_X_SPEED = 16;
    static constexpr float LEFT_MOVEMENT_BOUNDARY = 0;
    static constexpr float RIGHT_MOVEMENT_BOUNDARY = framework::WINDOW_WIDTH;
    static constexpr int TICKS_PER_SHOOT_CHANCE = 12;
    static constexpr int ALIEN_SHOOT_CHANCE = 3;
    static constexpr int SHOOT_X_JITTER = 3;
    static constexpr std::array<assets::Audio, 4> ARP_SOUNDS = {
        assets::Audio::Arp1,
        assets::Audio::Arp2,
        assets::Audio::Arp3,
        assets::Audio::Arp4,
    };

    std::function<void(entt::registry &, float, float)> add_projectile;
    std::function<void(entt::registry &)> rerack_aliens;
    std::mt19937 rand_gen;

    bool should_shoot_this_tick(entt::registry &ecs);
    void random_alien_shoot(entt::registry &ecs);
    bool should_move_this_tick(entt::registry &ecs, int num_aliens);
    bool did_hit_wall(entt::registry &ecs);
    void move(framework::ExecuteCtx &ctx, bool move_down, int num_aliens);
    void animate(entt::registry &ecs);

  public:
    Orchestration(
        std::function<void(entt::registry &, float, float)> add_projectile,
        std::function<void(entt::registry &)> rerack_aliens,
        unsigned int rand_seed
    );
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems::invaders