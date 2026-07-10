#pragma once

#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/event_broker.hpp"
#include "framework/ecs/system.hpp"
#include <memory>

namespace ecs {

class ECS {
  public:
    virtual ~ECS() = default;
    virtual Entity new_entity() = 0;
    virtual ComponentManager &components() = 0;
    virtual EventBroker &events() = 0;
    virtual void add_update_system(std::unique_ptr<System> system) = 0;
    virtual void add_draw_system(std::unique_ptr<System> system) = 0;
    virtual void register_to_systems(Entity entity) = 0;
    virtual void delete_entity(Entity entity) = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
};

} // namespace ecs
