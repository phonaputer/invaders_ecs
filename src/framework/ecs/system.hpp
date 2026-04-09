#pragma once

#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/entity.hpp"

namespace ecs {

class System {
  public:
    virtual ~System() = default;
    virtual void remove_entity(Entity entity) = 0;
    virtual bool matches_entity(Entity entity, ComponentManager &components) = 0;
    virtual void add_entity(Entity) = 0;
    virtual void update() = 0;
};

} // namespace ecs