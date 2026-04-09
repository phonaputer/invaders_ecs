#pragma once

#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/entity.hpp"
#include <memory>

namespace ecs {

class ECS {
  public:
    virtual ~ECS() = default;
    virtual Entity new_entity() = 0;
    virtual ComponentManager &components() = 0;
    virtual void register_to_systems(Entity entity) = 0;
    virtual void reregister_to_systems(Entity entity) = 0;
    virtual void delete_from_systems(Entity entity) = 0;
};

} // namespace ecs
