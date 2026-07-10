#include "gallia/systems/collision_detection.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "gallia/components/collision.hpp"
#include "gallia/components/position.hpp"
#include "gallia/events/collision_occurred.hpp"
#include <iterator>
#include <set>

namespace systems {

void CollisionDetection::remove_entity(ecs::Entity entity) {
  entities.erase(entity);
}

void CollisionDetection::add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) {
  if (components.has<components::Position>(entity) && components.has<components::Collision>(entity)) {
    entities.insert(entity);
  }
}

void CollisionDetection::execute(ecs::ECS &ecs) {
  for (auto l = entities.begin(); l != entities.end(); l++) {
    auto left_entity = *l;
    auto left_collision = ecs.components().get<components::Collision>(left_entity);
    auto left_position = ecs.components().get<components::Position>(left_entity);
    auto left_hitbox = Hitbox{
        .x = left_collision.hitbox_offset_x + left_position.x,
        .y = left_collision.hitbox_offset_y + left_position.y,
        .w = left_collision.hitbox_w,
        .h = left_collision.hitbox_h,
    };

    for (auto r = std::next(l); r != entities.end(); r++) {
      auto right_entity = *r;
      auto right_collision = ecs.components().get<components::Collision>(right_entity);
      auto right_position = ecs.components().get<components::Position>(right_entity);
      auto right_hitbox = Hitbox{
          .x = right_collision.hitbox_offset_x + right_position.x,
          .y = right_collision.hitbox_offset_y + right_position.y,
          .w = right_collision.hitbox_w,
          .h = right_collision.hitbox_h,
      };

      if (are_touching(left_hitbox, right_hitbox)) {
        ecs.events().push_back(events::CollisionOccurred{.who_am_i = left_entity, .who_i_hit = right_entity});
        ecs.events().push_back(events::CollisionOccurred{.who_am_i = right_entity, .who_i_hit = left_entity});
      }
    }
  }
}

bool CollisionDetection::are_touching(Hitbox right, Hitbox left) {
  // "left" above "right"
  if (left.y + left.h < right.y) {
    return false;
  }

  // "left" below "right"
  if (left.y > right.y + right.h) {
    return false;
  }

  // "left" left of "right"
  if (left.x + left.h < right.x) {
    return false;
  }

  // "left" right of "right"
  if (left.x > right.x + right.w) {
    return false;
  }

  return true;
}

} // namespace systems