#pragma once

#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "framework/game/player_input_manager.hpp"
#include <set>

namespace systems::player {

class Movement : public ecs::System {
  private:
    game::PlayerInputManager &player_input_manager;
    std::set<ecs::Entity> entities;

  public:
    Movement(game::PlayerInputManager &player_input_manager);
    void remove_entity(ecs::Entity entity) override;
    void add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) override;
    void execute(ecs::ECS &ecs) override;
};

} // namespace systems