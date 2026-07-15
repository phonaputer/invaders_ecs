#include "game/systems/collision_detection.hpp"
#include "framework/system.hpp"
#include "game/components/collision.hpp"
#include "game/components/position.hpp"
#include "game/events/collision_occurred.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iterator>
#include <set>

namespace systems {

void CollisionDetection::execute(framework::ExecuteCtx &ctx) {
  fill_buckets(ctx.ecs);

  for (const auto &[_, bucket] : hitbox_buckets) {
    check_collisions(ctx, bucket);
  }

  clear_buckets();
}

std::int32_t CollisionDetection::to_bucket_key(float x, float y) {
  auto bucket_x = static_cast<std::int16_t>(x / BUCKET_WIDTH);
  auto bucket_y = static_cast<std::int16_t>(y / BUCKET_HEIGHT);

  return ((std::int32_t)bucket_x << 8) | bucket_y;
}

void CollisionDetection::fill_buckets(entt::registry &ecs) {
  auto view = ecs.view<const components::Collision, const components::Position>();

  for (auto [entity, collision, position] : view.each()) {
    auto hitbox = Hitbox{
        .entity = entity,
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

void CollisionDetection::check_collisions(framework::ExecuteCtx &ctx, const std::vector<Hitbox> hitboxes) {
  for (size_t l = 0; l < hitboxes.size(); l++) {
    auto left = hitboxes.at(l);

    for (size_t r = l + 1; r < hitboxes.size(); r++) {
      auto right = hitboxes.at(r);

      if (are_touching(left, right)) {
        emit_collided_if_didnt_already(ctx, left.entity, right.entity);
      }
    }
  }
}

void CollisionDetection::emit_collided_if_didnt_already(
    framework::ExecuteCtx &ctx, entt::entity right, entt::entity left
) {
  for (const auto &event : ctx.events.get_all<events::CollisionOccurred>()) {
    if (event.who_am_i == right && event.who_i_hit == left) {
      return;
    }
    if (event.who_am_i == left && event.who_i_hit == right) {
      return;
    }
  }

  ctx.events.push_back(events::CollisionOccurred{.who_am_i = left, .who_i_hit = right});
  ctx.events.push_back(events::CollisionOccurred{.who_am_i = right, .who_i_hit = left});
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