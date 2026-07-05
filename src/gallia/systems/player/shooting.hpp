#pragma once

#include "core/point.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "framework/game/player_input_manager.hpp"
#include <functional>
#include <set>

namespace systems::player {

class Shooting : public ecs::System {
  private:
    game::PlayerInputManager &player_input_manager;
    std::set<ecs::Entity> entities;
    std::function<ecs::Entity(ecs::ECS &, core::Point)> add_projectile;
    std::set<ecs::Entity> active_projectiles;
    std::function<void(ecs::ECS &, ecs::Entity)> add_muzzle_flash;

  public:
    Shooting(
        game::PlayerInputManager &player_input_manager,
        std::function<ecs::Entity(ecs::ECS &ecs, core::Point)> add_projectile,
        std::function<void(ecs::ECS &, ecs::Entity)> add_muzzle_flash
    );
    void remove_entity(ecs::Entity entity) override;
    void add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) override;
    void execute(ecs::ECS &ecs) override;
};

} // namespace systems