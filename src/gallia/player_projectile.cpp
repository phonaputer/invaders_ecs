#include "gallia/player_projectile.hpp"
#include "core/point.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "gallia/components/animation.hpp"
#include "gallia/components/collision.hpp"
#include "gallia/components/deletable.hpp"
#include "gallia/components/lifetime.hpp"
#include "gallia/components/position.hpp"
#include "gallia/components/position_following.hpp"
#include "gallia/components/sprite.hpp"
#include "gallia/components/velocity.hpp"

namespace gallia {

ecs::Entity add_player_projectile_entity(ecs::ECS &ecs, core::Point starting_point) {
  auto entity = ecs.new_entity();

  ecs.components().set(
      entity,
      components::Position{
          .x = starting_point.x,
          .y = starting_point.y,
          .w = 16,
          .h = 16,
          .z = 101,
      }
  );
  ecs.components().set(
      entity,
      components::Collision{
          .active = true,
          .type = components::collision::Type::PlayerProjectile,
          .hitbox_offset_x = 7,
          .hitbox_offset_y = 6,
          .hitbox_w = 1,
          .hitbox_h = 4,
      }
  );
  ecs.components().set(
      entity,
      components::Sprite{
          .src_id = "invaders_spritesheet",
          .src_x = 112,
          .src_y = 16,
          .src_width = 16,
          .src_height = 16,
          .dst_width = 16,
          .dst_height = 16,
      }
  );
  ecs.components().set(
      entity,
      components::Velocity{
          .x = 0,
          .y = -4,
      }
  );
  ecs.components().set(
      entity,
      components::Deleteable{
          .is_deleted = false,
      }
  );

  ecs.register_to_systems(entity);

  return entity;
}

void add_player_muzzle_flash_entity(ecs::ECS &ecs, ecs::Entity shooter) {
  auto entity = ecs.new_entity();

  ecs.components().set(
      entity,
      components::Position{
          .x = 0,
          .y = 0,
          .w = 16,
          .h = 16,
          .z = 102,
      }
  );
  ecs.components().set(
      entity,
      components::Sprite{
          .src_id = "invaders_spritesheet",
          .src_x = 48,
          .src_y = 32,
          .src_width = 16,
          .src_height = 16,
          .dst_width = 16,
          .dst_height = 16,
      }
  );
  ecs.components().set(
      entity,
      components::PositionFollowing{
          .leader = shooter,
          .x_offset = -2,
          .y_offset = -2,
      }
  );
  ecs.components().set(
      entity,
      components::Deleteable{
          .is_deleted = false,
      }
  );
  std::vector<components::AnimationFrame> frames = {{3, 2}, {4, 2}};
  ecs.components().set(
      entity,
      components::Animation{
          .playing = true,
          .play_reversed = false,
          .frames = std::move(frames),
          .cur_frame = 0,
          .ticks_per_frame = 10,
          .tick_counter = 0,
      }
  );
  ecs.components().set(
      entity,
      components::Lifetime{
          .ticks = 20,
          .tick_counter = 0,
      }
  );

  ecs.register_to_systems(entity);
}

} // namespace gallia