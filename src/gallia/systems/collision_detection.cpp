#include "gallia/systems/collision_detection.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "gallia/components/collision.hpp"
#include "gallia/components/position.hpp"
#include <set>
#include <vector>

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
  active_entities_this_tick.clear();
  for (const auto &entity : entities) {
    if (ecs.components().get<components::Collision>(entity).active) {
      active_entities_this_tick.push_back(entity);
    }
  }

  for (const auto &entity : active_entities_this_tick) {
    auto col = ecs.components().get<components::Collision>(entity);
    col.hit_something_this_tick = false;
    col.type_of_what_i_hit = "";
    ecs.components().set(entity, col);
  }

  for (size_t l = 0; l < active_entities_this_tick.size(); l++) {
    auto left_entity = active_entities_this_tick.at(l);

    auto left_collision = ecs.components().get<components::Collision>(left_entity);
    if (left_collision.hit_something_this_tick) {
      continue;
    }

    auto left_position = ecs.components().get<components::Position>(left_entity);
    auto left_hitbox = Hitbox{
        .x = left_collision.hitbox_offset_x + left_position.x,
        .y = left_collision.hitbox_offset_y + left_position.y,
        .w = left_collision.hitbox_w,
        .h = left_collision.hitbox_h,
    };

    for (size_t r = l + 1; r < active_entities_this_tick.size(); r++) {
      auto right_entity = active_entities_this_tick.at(r);
      auto right_collision = ecs.components().get<components::Collision>(right_entity);
      if (right_collision.hit_something_this_tick) {
        continue;
      }

      auto right_position = ecs.components().get<components::Position>(right_entity);
      auto right_hitbox = Hitbox{
          .x = right_collision.hitbox_offset_x + right_position.x,
          .y = right_collision.hitbox_offset_y + right_position.y,
          .w = right_collision.hitbox_w,
          .h = right_collision.hitbox_h,
      };

      if (are_touching(left_hitbox, right_hitbox)) {
        left_collision.hit_something_this_tick = true;
        left_collision.type_of_what_i_hit = right_collision.type;
        ecs.components().set(left_entity, left_collision);

        right_collision.hit_something_this_tick = true;
        right_collision.type_of_what_i_hit = left_collision.type;
        ecs.components().set(right_entity, right_collision);
        break;
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