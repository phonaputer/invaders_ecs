#include "gallia/systems/collision_detection.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "gallia/components/collision.hpp"
#include "gallia/components/position.hpp"
#include "gallia/events/collision_occurred.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
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
  fill_buckets(ecs);

  for (const auto &[_, bucket] : hitbox_buckets) {
    check_collisions(ecs, bucket);
  }

  clear_buckets();
}

std::int32_t CollisionDetection::to_bucket_key(float x, float y) {
  auto bucket_x = static_cast<std::int16_t>(x / BUCKET_WIDTH);
  auto bucket_y = static_cast<std::int16_t>(y / BUCKET_HEIGHT);

  return ((std::int32_t)bucket_x << 8) | bucket_y;
}

void CollisionDetection::fill_buckets(ecs::ECS &ecs) {
  for (auto e = entities.begin(); e != entities.end(); e++) {
    auto collision = ecs.components().get<components::Collision>(*e);
    auto position = ecs.components().get<components::Position>(*e);
    auto hitbox = Hitbox{
        .entity = *e,
        .x = collision.hitbox_offset_x + position.x,
        .y = collision.hitbox_offset_y + position.y,
        .w = collision.hitbox_w,
        .h = collision.hitbox_h,
    };

    add_to_bucket(hitbox, hitbox.x, hitbox.y);
    add_to_bucket(hitbox, hitbox.x, std::ceil(hitbox.y + hitbox.h));
    add_to_bucket(hitbox, std::ceil(hitbox.x + hitbox.w), hitbox.y);
    add_to_bucket(hitbox, std::ceil(hitbox.x + hitbox.w), std::ceil(hitbox.y + hitbox.h));
  }
}

void CollisionDetection::add_to_bucket(Hitbox hitbox, float x, float y) {
  auto &bucket = hitbox_buckets[to_bucket_key(x, y)];

  for (const auto &hb : bucket) {
    if (hb.entity == hitbox.entity) {
      return;
    }
  }

  bucket.push_back(hitbox);
}

// It's ok to clear these without deleting them from the map since the size of
// the canvas never changes meaning the list of buckets is fixed.
void CollisionDetection::clear_buckets() {
  for (auto &[_, v] : hitbox_buckets) {
    v.clear();
  }
}

void CollisionDetection::check_collisions(ecs::ECS &ecs, const std::vector<Hitbox> hitboxes) {
  for (size_t l = 0; l < hitboxes.size(); l++) {
    auto left = hitboxes.at(l);

    for (size_t r = l + 1; r < hitboxes.size(); r++) {
      auto right = hitboxes.at(r);

      if (are_touching(left, right)) {
        emit_collided_if_didnt_already(ecs, left.entity, right.entity);
      }
    }
  }
}

void CollisionDetection::emit_collided_if_didnt_already(ecs::ECS &ecs, ecs::Entity right, ecs::Entity left) {
  for (const auto &event : ecs.events().get_all<events::CollisionOccurred>()) {
    if (event.who_am_i == right && event.who_i_hit == left) {
      return;
    }
    if (event.who_am_i == left && event.who_i_hit == right) {
      return;
    }
  }

  ecs.events().push_back(events::CollisionOccurred{.who_am_i = left, .who_i_hit = right});
  ecs.events().push_back(events::CollisionOccurred{.who_am_i = right, .who_i_hit = left});
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