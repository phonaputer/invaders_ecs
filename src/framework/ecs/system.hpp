#pragma once

#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/entity.hpp"

namespace ecs {

class System {
  public:
    virtual ~System() = default;
    virtual void remove_entity(Entity entity) = 0;
    virtual void add_entity_if_matches(Entity entity, ComponentManager &components) = 0;
    virtual void execute(ComponentManager &components) = 0;
};

} // namespace ecs