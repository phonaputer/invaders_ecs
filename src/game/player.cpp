#include "game/player.hpp"
#include "core/point.hpp"
#include "framework/constants.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "game/components/animation.hpp"
#include "game/components/collision.hpp"
#include "game/components/damage_dealer.hpp"
#include "game/components/damage_type_enum.hpp"
#include "game/components/deletable.hpp"
#include "game/components/hitpoints.hpp"
#include "game/components/lifetime.hpp"
#include "game/components/player/movement.hpp"
#include "game/components/player/shooting.hpp"
#include "game/components/position.hpp"
#include "game/components/position_following.hpp"
#include "game/components/sprite.hpp"
#include "game/components/velocity.hpp"
#include <vector>

namespace game {

void add_player_entity(ecs::ECS &ecs) {
  auto entity = ecs.new_entity();

  ecs.components().set(
      entity,
      components::Deleteable{
          .is_deleted = false,
      }
  );
  ecs.components().set(
      entity,
      components::Position{
          .x = framework::WINDOW_WIDTH / 2 - 8,
          .y = framework::WINDOW_HEIGHT - 35,
          .w = 16,
          .h = 16,
          .z = 100,
      }
  );
  ecs.components().set(
      entity,
      components::Sprite{
          .src_id = "invaders_spritesheet",
          .src_x = 0,
          .src_y = 32,
          .src_width = 16,
          .src_height = 16,
          .dst_width = 16,
          .dst_height = 16,
      }
  );
  std::vector<components::AnimationFrame> frames = {{0, 2}, {1, 2}, {2, 2}};
  ecs.components().set(
      entity,
      components::Animation{
          .playing = false,
          .play_reversed = false,
          .frames = std::move(frames),
          .cur_frame = 0,
          .ticks_per_frame = 4,
          .tick_counter = 0,
      }
  );
  ecs.components().set(
      entity,
      components::player::Movement{
          .x_speed = 1,
      }
  );
  ecs.components().set(
      entity,
      components::Collision{
          .hitbox_offset_x = 0,
          .hitbox_offset_y = 7,
          .hitbox_w = 15,
          .hitbox_h = 5,
      }
  );
  ecs.components().set(
      entity,
      components::player::Shooting{
          .ticks_per_shot = 8,
          .shot_clock = 0,
          .shot_offset_x = -2,
          .shot_offset_y = -2,
          .max_simultaneous_shots = 3,
      }
  );
  components::DamageTypeSet susceptible_damage_types;
  susceptible_damage_types.set(components::damage_type_to_index(components::DamageType::Alien_Projectile));
  ecs.components().set(
      entity,
      components::Hitpoints{
          .susceptible_to = susceptible_damage_types,
          .cur_hitpoints = 1,
      }
  );
  components::DamageTypeSet deal_damage_types;
  deal_damage_types.set(components::damage_type_to_index(components::DamageType::Player));
  ecs.components().set(
      entity,
      components::DamageDealer{
          .type = deal_damage_types,
          .amount = 1,
      }
  );

  ecs.register_to_systems(entity);
}

ecs::Entity add_player_projectile_entity(ecs::ECS &ecs, core::Point starting_point) {
  auto entity = ecs.new_entity();

  components::DamageTypeSet susceptible_damage_types;
  susceptible_damage_types.set(components::damage_type_to_index(components::DamageType::Alien));
  susceptible_damage_types.set(components::damage_type_to_index(components::DamageType::Alien_Projectile));
  susceptible_damage_types.set(components::damage_type_to_index(components::DamageType::Fortress));
  ecs.components().set(
      entity,
      components::Hitpoints{
          .susceptible_to = susceptible_damage_types,
          .cur_hitpoints = 1,
      }
  );
  components::DamageTypeSet deal_damage_types;
  deal_damage_types.set(components::damage_type_to_index(components::DamageType::Player_Projectile));
  ecs.components().set(
      entity,
      components::DamageDealer{
          .type = deal_damage_types,
          .amount = 1,
      }
  );
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

void add_player_explosion_entity(ecs::ECS &ecs, core::Point position, unsigned int lifetime) {
  auto entity = ecs.new_entity();

  ecs.components().set(
      entity,
      components::Position{
          .x = position.x,
          .y = position.y,
          .w = 16,
          .h = 16,
          .z = 99,
      }
  );
  ecs.components().set(
      entity,
      components::Sprite{
          .src_id = "invaders_spritesheet",
          .src_x = 16,
          .src_y = 48,
          .src_width = 16,
          .src_height = 16,
          .dst_width = 16,
          .dst_height = 16,
      }
  );
  ecs.components().set(
      entity,
      components::Deleteable{
          .is_deleted = false,
      }
  );
  std::vector<components::AnimationFrame> frames = {{1, 3}, {0, 3}, {1, 3}, {2, 3}};
  ecs.components().set(
      entity,
      components::Animation{
          .playing = true,
          .play_reversed = false,
          .frames = std::move(frames),
          .cur_frame = 0,
          .ticks_per_frame = 5,
          .tick_counter = 0,
      }
  );
  ecs.components().set(
      entity,
      components::Lifetime{
          .ticks = lifetime,
      }
  );

  ecs.register_to_systems(entity);
}

} // namespace game