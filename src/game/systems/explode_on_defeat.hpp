#pragma once

#include "core/point.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include <functional>
#include <set>

namespace systems {

class ExplodeOnDefeat : public ecs::System {
  private:
    std::set<ecs::Entity> entities;

    std::function<void(ecs::ECS &, core::Point)> add_explosion;

  public:
    ExplodeOnDefeat(std::function<void(ecs::ECS &, core::Point)> add_explosion);
    void remove_entity(ecs::Entity entity) override;
    void add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) override;
    void execute(ecs::ECS &ecs) override;
};

} // namespace systems