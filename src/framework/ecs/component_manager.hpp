#pragma once

#include "framework/ecs/entity.hpp"
#include <any>
#include <typeindex>
#include <unordered_map>

namespace ecs {

class ComponentManager {
  private:
    std::unordered_map<std::type_index, std::unordered_map<Entity, std::any>> components;

  public:
    template <typename T> void set(ecs::Entity entity, T component);
    template <typename T> T get(ecs::Entity entity) const;
    template <typename T> bool has(ecs::Entity entity);
    template <typename T> void delete_component(ecs::Entity entity);
    void delete_entity(ecs::Entity entity);
    void clear();
};

} // namespace ecs

#include "framework/ecs/component_manager.tpp"