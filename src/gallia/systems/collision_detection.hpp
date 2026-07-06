#pragma once

#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "gallia/components/collision.hpp"
#include <set>
#include <vector>

namespace systems {

struct Hitbox {
    float x;
    float y;
    float w;
    float h;
};

class CollisionDetection : public ecs::System {
  private:
    std::vector<ecs::Entity> active_entities_this_tick;
    std::set<ecs::Entity> entities;

    bool are_touching(Hitbox right, Hitbox left);

  public:
    void remove_entity(ecs::Entity entity) override;
    void add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) override;
    void execute(ecs::ECS &ecs) override;
};

} // namespace systems