#pragma once

#include "framework/ecs/entity.hpp"
#include <any>
#include <concepts>
#include <ranges>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

namespace ecs {

using ComponentID = std::type_index;
using ArchetypeID = unsigned int;
using TypeSig = std::string; // TODO improve this
using Column = std::vector<std::any>;

struct Archetype {
    ArchetypeID id;
    TypeSig signature;
    std::vector<Column> components;
};

class MertensECS {
  private:
    std::unordered_map<Entity, std::unordered_map<std::type_index, std::vector<std::any>>> components;

    std::unordered_map<TypeSig, Archetype> archetypes_by_signature;
    std::unordered_map<Entity, Archetype &> entity_archetypes;

  public:
    template <typename T> void set(Entity entity, T component);
    template <typename... Ts> bool has_all(Entity entity);
    template <typename Callback, typename... CallbackArgs>
      requires std::invocable<Callback, Entity, CallbackArgs...>
    void invoke_on(Callback &&cb, CallbackArgs... args);

    // {
    //   std::forward<Callback>(cb)(123, std::forward<CallbackArgs>(args)...);
    // }
};

} // namespace ecs
