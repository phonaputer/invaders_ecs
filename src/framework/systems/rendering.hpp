#pragma once

#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "framework/game/renderer.hpp"
#include <memory>
#include <set>

namespace systems {

class Rendering : public ecs::System {
  private:
    std::set<ecs::Entity> entities;
    std::shared_ptr<game::Renderer> renderer;

  public:
    Rendering(std::shared_ptr<game::Renderer> renderer);
    void remove_entity(ecs::Entity entity) override;
    bool matches_entity(ecs::Entity entity, ecs::ComponentManager &components) override;
    void add_entity(ecs::Entity entity) override;
    void execute(ecs::ComponentManager &components) override;
};

} // namespace systems