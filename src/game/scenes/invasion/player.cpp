#include "game/scenes/invasion/player.hpp"
#include "framework/constants.hpp"
#include "game/assets/asset_enums.hpp"
#include "game/scenes/invasion/components/animation.hpp"
#include "game/scenes/invasion/components/collision.hpp"
#include "game/scenes/invasion/components/damage_dealer.hpp"
#include "game/scenes/invasion/components/damage_type_enum.hpp"
#include "game/scenes/invasion/components/delete_on_gameover.hpp"
#include "game/scenes/invasion/components/hitpoints.hpp"
#include "game/scenes/invasion/components/interacts_on_collide.hpp"
#include "game/scenes/invasion/components/lifetime.hpp"
#include "game/scenes/invasion/components/player_movement.hpp"
#include "game/scenes/invasion/components/player_shooting.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include "game/scenes/invasion/components/position_following.hpp"
#include "game/scenes/invasion/components/sprite.hpp"
#include "game/scenes/invasion/components/velocity.hpp"
#include <vector>

namespace invasion {

void add_player_entity(entt::registry &ecs) {
  const auto entity = ecs.create();

  ecs.emplace<components::DeleteOnGameOver>(entity);
  ecs.emplace<components::Sprite>(
      entity,
      components::Sprite{
          .image = assets::Image::InvadersSpritesheet,
          .src_x = 0,
          .src_y = 32,
          .src_width = 16,
          .src_height = 16,
          .dst_width = 16,
          .dst_height = 16,
      }
  );
  std::vector<components::AnimationFrame> frames = {{0, 2}, {1, 2}, {2, 2}};
  ecs.emplace<components::Animation>(
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
  ecs.emplace<components::PlayerMovement>(
      entity,
      components::PlayerMovement{
          .x_speed = 1,
      }
  );
  ecs.emplace<components::Collision>(
      entity,
      components::Collision{
          .hitbox_offset_x = 0,
          .hitbox_offset_y = 7,
          .hitbox_w = 15,
          .hitbox_h = 5,
      }
  );
  ecs.emplace<components::PlayerShooting>(
      entity,
      components::PlayerShooting{
          .ticks_per_shot = 8,
          .shot_clock = 0,
          .shot_offset_x = -2,
          .shot_offset_y = -2,
          .max_simultaneous_shots = 3,
      }
  );
  components::DamageTypeSet susceptible_damage_types;
  susceptible_damage_types.set(components::damage_type_to_index(components::DamageType::Alien_Projectile));
  ecs.emplace<components::Hitpoints>(
      entity,
      components::Hitpoints{
          .susceptible_to = susceptible_damage_types,
          .cur_hitpoints = 1,
      }
  );
  components::DamageTypeSet deal_damage_types;
  deal_damage_types.set(components::damage_type_to_index(components::DamageType::Player));
  ecs.emplace<components::DamageDealer>(
      entity,
      components::DamageDealer{
          .type = deal_damage_types,
          .amount = 1,
      }
  );

  ecs.emplace<components::Position>(
      entity,
      components::Position{
          .x = framework::WINDOW_WIDTH / 2 - 8,
          .y = framework::WINDOW_HEIGHT - 35,
          .w = 16,
          .h = 16,
          .z = 100,
      }
  );
  ecs.sort<components::Position>([](const auto &left, const auto &right) { return left.z < right.z; });
}

entt::entity add_player_projectile_entity(entt::registry &ecs, float x, float y) {
  const auto entity = ecs.create();

  ecs.emplace<components::InteractsOnCollide>(entity);
  ecs.emplace<components::DeleteOnGameOver>(entity);
  components::DamageTypeSet susceptible_damage_types;
  susceptible_damage_types.set(components::damage_type_to_index(components::DamageType::Alien));
  susceptible_damage_types.set(components::damage_type_to_index(components::DamageType::Alien_Projectile));
  susceptible_damage_types.set(components::damage_type_to_index(components::DamageType::Fortress));
  ecs.emplace<components::Hitpoints>(
      entity,
      components::Hitpoints{
          .susceptible_to = susceptible_damage_types,
          .cur_hitpoints = 1,
      }
  );
  components::DamageTypeSet deal_damage_types;
  deal_damage_types.set(components::damage_type_to_index(components::DamageType::Player_Projectile));
  ecs.emplace<components::DamageDealer>(
      entity,
      components::DamageDealer{
          .type = deal_damage_types,
          .amount = 1,
      }
  );
  ecs.emplace<components::Collision>(
      entity,
      components::Collision{
          .hitbox_offset_x = 7,
          .hitbox_offset_y = 6,
          .hitbox_w = 1,
          .hitbox_h = 4,
      }
  );
  ecs.emplace<components::Sprite>(
      entity,
      components::Sprite{
          .image = assets::Image::InvadersSpritesheet,
          .src_x = 112,
          .src_y = 16,
          .src_width = 16,
          .src_height = 16,
          .dst_width = 16,
          .dst_height = 16,
      }
  );
  ecs.emplace<components::Velocity>(
      entity,
      components::Velocity{
          .x = 0,
          .y = -4,
      }
  );

  ecs.emplace<components::Position>(
      entity,
      components::Position{
          .x = x,
          .y = y,
          .w = 16,
          .h = 16,
          .z = 99,
      }
  );
  ecs.sort<components::Position>([](const auto &left, const auto &right) { return left.z < right.z; });

  return entity;
}

void add_player_muzzle_flash_entity(entt::registry &ecs, entt::entity shooter) {
  const auto entity = ecs.create();

  ecs.emplace<components::DeleteOnGameOver>(entity);
  ecs.emplace<components::Position>(
      entity,
      components::Position{
          .x = 0,
          .y = 0,
          .w = 16,
          .h = 16,
          .z = 110,
      }
  );
  ecs.emplace<components::Sprite>(
      entity,
      components::Sprite{
          .image = assets::Image::InvadersSpritesheet,
          .src_x = 48,
          .src_y = 32,
          .src_width = 16,
          .src_height = 16,
          .dst_width = 16,
          .dst_height = 16,
      }
  );
  ecs.emplace<components::PositionFollowing>(
      entity,
      components::PositionFollowing{
          .leader = shooter,
          .x_offset = -2,
          .y_offset = -2,
      }
  );
  std::vector<components::AnimationFrame> frames = {{3, 2}, {4, 2}};
  ecs.emplace<components::Animation>(
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
  ecs.emplace<components::Lifetime>(
      entity,
      components::Lifetime{
          .ticks = 20,
          .tick_counter = 0,
      }
  );
}

void add_player_explosion_entity(entt::registry &ecs, float x, float y, unsigned int lifetime) {
  const auto entity = ecs.create();

  ecs.emplace<components::DeleteOnGameOver>(entity);
  ecs.emplace<components::Position>(
      entity,
      components::Position{
          .x = x,
          .y = y,
          .w = 16,
          .h = 16,
          .z = 100,
      }
  );
  ecs.emplace<components::Sprite>(
      entity,
      components::Sprite{
          .image = assets::Image::InvadersSpritesheet,
          .src_x = 16,
          .src_y = 48,
          .src_width = 16,
          .src_height = 16,
          .dst_width = 16,
          .dst_height = 16,
      }
  );
  std::vector<components::AnimationFrame> frames = {{1, 3}, {0, 3}, {1, 3}, {2, 3}};
  ecs.emplace<components::Animation>(
      entity,
      components::Animation{
          .playing = true,
          .play_reversed = false,
          .frames = std::move(frames),
          .cur_frame = 0,
          .ticks_per_frame = 5,
          .tick_counter = 0,
          .pausable = false,
      }
  );
  ecs.emplace<components::Lifetime>(
      entity,
      components::Lifetime{
          .ticks = lifetime,
      }
  );
}

} // namespace invasion