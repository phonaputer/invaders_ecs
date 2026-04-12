#pragma once

#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/entity.hpp"
#include <any>
#include <typeindex>
#include <unordered_map>

namespace ecs {

template <typename T> void ComponentManager::set(ecs::Entity entity, T component) {
  components[std::type_index(typeid(T))].insert_or_assign(entity, component);
}

template <typename T> T ComponentManager::get(ecs::Entity entity) const {
  auto any_value = components.at(std::type_index(typeid(T))).at(entity);

  return std::any_cast<T>(any_value);
}

template <typename T> bool ComponentManager::has(ecs::Entity entity) {
  if (!components.contains(std::type_index(typeid(T)))) {
    return false;
  }

  return components.at(std::type_index(typeid(T))).contains(entity);
}

template <typename T> void ComponentManager::delete_component(ecs::Entity entity) {
  components.at(std::type_index(typeid(T))).erase(entity);
}

inline void ComponentManager::delete_entity(ecs::Entity entity) {
  for (auto &[_, component] : components) {
    component.erase(entity);
  }
}

inline void ComponentManager::clear() {
  components.clear();
}

} // namespace ecs