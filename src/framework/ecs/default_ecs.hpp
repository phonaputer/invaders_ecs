#pragma once

#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include <memory>
#include <vector>

namespace ecs {

class DefaultECS : public ECS {
  private:
    unsigned long cur_entity_id = 0; // note: this is not thread-safe
    std::vector<std::unique_ptr<ecs::System>> update_systems;
    std::vector<std::unique_ptr<ecs::System>> draw_systems;
    std::unique_ptr<ComponentManager> component_manager;

  public:
    DefaultECS(std::unique_ptr<ComponentManager> component_manager);
    Entity new_entity() override;
    ComponentManager &components() override;
    void add_update_system(std::unique_ptr<System> system) override;
    void add_draw_system(std::unique_ptr<System> system) override;
    void register_to_systems(Entity entity) override;
    void delete_from_systems(Entity entity) override;
    void update() override;
    void draw() override;
};

} // namespace ecs
