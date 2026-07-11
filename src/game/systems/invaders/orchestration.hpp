#pragma once

#include "core/point.hpp"
#include "framework/constants.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include <functional>
#include <random>
#include <vector>

namespace systems::invaders {

class Orchestration : public ecs::System {
  private:
    static constexpr int BASE_TICKS_PER_MOVE = 58;
    static constexpr float X_SPEED = 5;
    static constexpr float Y_SPEED = 5;
    static constexpr float LEFT_MOVEMENT_BOUNDARY = 0;
    static constexpr float RIGHT_MOVEMENT_BOUNDARY = game::WINDOW_WIDTH;
    static constexpr int TICKS_PER_SHOOT_CHANCE = 45;
    static constexpr int ALIEN_SHOOT_CHANCE = 3;

    bool paused = false;

    bool move_right = true;
    int tick_counter = 0;
    std::function<void(ecs::ECS &, core::Point)> add_projectile;
    std::mt19937 rand_gen;
    unsigned int shoot_counter = 0;

    std::vector<ecs::Entity> entities;

    bool should_shoot_this_tick();
    void random_alien_shoot(ecs::ECS &ecs);
    bool should_move_this_tick();
    bool did_hit_wall(ecs::ECS &ecs, ecs::Entity entity);
    void move(ecs::ECS &ecs, ecs::Entity entity, bool move_down);
    void animate(ecs::ECS &ecs, ecs::Entity entity);

  public:
    Orchestration(std::function<void(ecs::ECS &, core::Point)> add_projectile, unsigned int rand_seed);
    void remove_entity(ecs::Entity entity) override;
    void add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) override;
    void execute(ecs::ECS &ecs) override;
};

} // namespace systems::invaders