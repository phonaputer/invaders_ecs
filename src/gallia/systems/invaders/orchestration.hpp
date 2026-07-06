#pragma once

#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "framework/game/constants.hpp"
#include <set>

namespace systems::invaders {

class Orchestration : public ecs::System {
  private:
    static constexpr int BASE_TICKS_PER_MOVE = 58;
    static constexpr float X_SPEED = 8;
    static constexpr float Y_SPEED = 8;
    static constexpr float LEFT_MOVEMENT_BOUNDARY = 16;
    static constexpr float RIGHT_MOVEMENT_BOUNDARY = game::WINDOW_WIDTH - 16;

    bool move_right = true;
    int tick_counter = 0;
    std::set<ecs::Entity> entities;
    std::set<ecs::Entity> active_entities_this_tick;

    bool should_move_this_tick();
    bool did_hit_wall(ecs::ECS &ecs, ecs::Entity entity);
    void move(ecs::ECS &ecs, ecs::Entity entity, bool move_down);
    void animate(ecs::ECS &ecs, ecs::Entity entity);

  public:
    void remove_entity(ecs::Entity entity) override;
    void add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) override;
    void execute(ecs::ECS &ecs) override;
};

} // namespace systems::invaders