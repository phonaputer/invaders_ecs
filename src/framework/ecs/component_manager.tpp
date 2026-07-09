#pragma once

#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/entity.hpp"
#include <any>
#include <cassert>
#include <typeindex>
#include <unordered_map>

namespace ecs {

template <typename T> void ComponentManager::set(ecs::Entity entity, T component) {
  components[std::type_index(typeid(T))].insert_or_assign(entity, component);
}

template <typename T> T ComponentManager::get(ecs::Entity entity) const {
  assert(components.contains(std::type_index(typeid(T))));
  assert(components.at(std::type_index(typeid(T))).contains(entity));

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
  if (!components.contains(std::type_index(typeid(T)))) {
    return;
  }

  components.at(std::type_index(typeid(T))).erase(entity);
}

inline void ComponentManager::delete_entity(ecs::Entity entity) {
  for (auto &[_, component] : components) {
    component.erase(entity);
  }
}

template <typename T> void ComponentManager::set_singleton(T component) {
  singleton_components[std::type_index(typeid(T))] = component;
}

template <typename T> T ComponentManager::get_singleton() const {
  assert(singleton_components.contains(std::type_index(typeid(T))));

  auto any_value = singleton_components.at(std::type_index(typeid(T)));

  return std::any_cast<T>(any_value);
}

} // namespace ecs