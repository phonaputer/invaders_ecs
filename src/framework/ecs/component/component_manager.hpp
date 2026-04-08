#pragma once

#include "framework/ecs/entity.hpp"

namespace component {

template <typename T> class Manager {
  public:
    void set(ecs::EntityID entity, T component);
    T get(ecs::EntityID entity) const;
    void del(ecs::EntityID entity);
};

} // namespace component
