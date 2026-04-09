#pragma once

#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include <memory>
#include <vector>

namespace ecs {

class DefaultECS : ECS {
  private:
    unsigned long cur_entity_id = 0; // note: this is not thread-safe
    std::vector<std::unique_ptr<ecs::System>> systems;
    std::unique_ptr<ComponentManager> component_manager;

  public:
    DefaultECS(std::vector<std::unique_ptr<ecs::System>> systems, std::unique_ptr<ComponentManager> component_manager);
    Entity new_entity() override;
    ComponentManager &components() override;
    void register_to_systems(Entity entity) override;
    void reregister_to_systems(Entity entity) override;
    void delete_from_systems(Entity entity) override;
};

} // namespace ecs
