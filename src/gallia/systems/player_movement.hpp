#pragma once

#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "framework/game/player_input_manager.hpp"
#include "framework/game/renderer.hpp"
#include <set>

namespace systems {

class PlayerMovement : public ecs::System {
  private:
    game::PlayerInputManager &player_input_manager;
    std::set<ecs::Entity> entities;

  public:
    PlayerMovement(game::PlayerInputManager &player_input_manager);
    void remove_entity(ecs::Entity entity) override;
    void add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) override;
    void execute(ecs::ComponentManager &components) override;
};

} // namespace systems