#pragma once

#include "framework/ecs/component/all_components.hpp"
#include "framework/ecs/entity.hpp"

namespace ecs {

class EntityManager {
  public:
    EntityID new_entity() const;
    component::All components() const;
};

} // namespace ecs
