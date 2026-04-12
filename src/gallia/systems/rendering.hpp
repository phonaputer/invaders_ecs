#pragma once

#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "framework/game/renderer.hpp"
#include <set>

namespace systems {

class Rendering : public ecs::System {
  private:
    std::set<ecs::Entity> entities;
    game::Renderer &renderer;

  public:
    Rendering(game::Renderer &renderer);
    void remove_entity(ecs::Entity entity) override;
    void add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) override;
    void execute(ecs::ComponentManager &components) override;
};

} // namespace systems