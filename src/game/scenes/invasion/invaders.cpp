#include "game/scenes/invasion/invaders.hpp"
#include "framework/constants.hpp"
#include "game/assets/asset_enums.hpp"
#include "game/scenes/invasion/components/animation.hpp"
#include "game/scenes/invasion/components/collision.hpp"
#include "game/scenes/invasion/components/damage_dealer.hpp"
#include "game/scenes/invasion/components/damage_type_enum.hpp"
#include "game/scenes/invasion/components/deletable.hpp"
#include "game/scenes/invasion/components/delete_on_gameover.hpp"
#include "game/scenes/invasion/components/explode_on_defeat.hpp"
#include "game/scenes/invasion/components/hitpoints.hpp"
#include "game/scenes/invasion/components/interacts_on_collide.hpp"
#include "game/scenes/invasion/components/invader.hpp"
#include "game/scenes/invasion/components/lifetime.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include "game/scenes/invasion/components/singleton/invader_orchestration.hpp"
#include "game/scenes/invasion/components/sound_on_defeat.hpp"
#include "game/scenes/invasion/components/sprite.hpp"
#include "game/scenes/invasion/components/step_animation.hpp"
#include "game/scenes/invasion/components/velocity.hpp"
#include <entt.hpp>

namespace invasion {

struct AddInvaderArgs {
    float start_x;
    float start_y;
    std::vector<components::StepAnimationFrame> frames;
    float hitbox_offset_x;
    float hitfox_offset_y;
    float hitbox_w;
    float hitbox_h;
    unsigned int score;
};

void add_invader_entity(entt::registry &ecs, AddInvaderArgs args) {
  const auto entity = ecs.create();

  ecs.emplace<components::SoundOnDefeat>(entity, components::SoundOnDefeat{.audio = assets::Audio::AlienExplosion});

  ecs.emplace<components::ExplodeOnDefeat>(entity);

  ecs.emplace<components::Invader>(entity);

  ecs.emplace<components::DeleteOnGameOver>(entity);

  components::DamageTypeSet susceptible_damage_types;
  susceptible_damage_types.set(components::damage_type_to_index(components::DamageType::Player_Projectile));
  ecs.emplace<components::Hitpoints>(
      entity,
      components::Hitpoints{
          .susceptible_to = susceptible_damage_types,
          .cur_hitpoints = 1,
          .grants_score = true,
          .score_value = args.score,
      }
  );

  components::DamageTypeSet deal_damage_types;
  deal_damage_types.set(components::damage_type_to_index(components::DamageType::Alien));
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
          .x = args.start_x,
          .y = args.start_y,
          .w = 16,
          .h = 16,
          .z = 90,
      }
  );

  ecs.emplace<components::Collision>(
      entity,
      components::Collision{
          .hitbox_offset_x = args.hitbox_offset_x,
          .hitbox_offset_y = args.hitfox_offset_y,
          .hitbox_w = args.hitbox_w,
          .hitbox_h = args.hitbox_h,
      }
  );

  auto frame_zero = args.frames.at(0);

  ecs.emplace<components::Sprite>(
      entity,
      components::Sprite{
          .image = assets::Image::InvadersSpritesheet,
          .src_x = float(frame_zero.x) * 16,
          .src_y = float(frame_zero.y) * 16,
          .src_width = 16,
          .src_height = 16,
          .dst_width = 14,
          .dst_height = 14,
      }
  );

  ecs.emplace<components::StepAnimation>(
      entity,
      components::StepAnimation{
          .frames = std::move(args.frames),
          .cur_frame = 0,
      }
  );
}

void add_octopus(entt::registry &ecs, float start_x, float start_y) {
  std::vector<components::StepAnimationFrame> frames = {{6, 0}, {7, 0}};

  add_invader_entity(
      ecs,
      AddInvaderArgs{
          .start_x = start_x,
          .start_y = start_y,
          .frames = std::move(frames),
          .hitbox_offset_x = 0,
          .hitfox_offset_y = 3,
          .hitbox_w = 14,
          .hitbox_h = 8,
          .score = 40,
      }
  );
}

void add_jellyfish(entt::registry &ecs, float start_x, float start_y) {
  std::vector<components::StepAnimationFrame> frames = {{1, 0}, {0, 0}, {1, 0}, {2, 0}};

  add_invader_entity(
      ecs,
      AddInvaderArgs{
          .start_x = start_x,
          .start_y = start_y,
          .frames = std::move(frames),
          .hitbox_offset_x = 1,
          .hitfox_offset_y = 3,
          .hitbox_w = 12,
          .hitbox_h = 8,
          .score = 30,
      }
  );
}

void add_crab(entt::registry &ecs, float start_x, float start_y) {
  std::vector<components::StepAnimationFrame> frames = {{1, 1}, {0, 1}, {1, 1}, {2, 1}};

  add_invader_entity(
      ecs,
      AddInvaderArgs{
          .start_x = start_x,
          .start_y = start_y,
          .frames = std::move(frames),
          .hitbox_offset_x = 2,
          .hitfox_offset_y = 2,
          .hitbox_w = 10,
          .hitbox_h = 9,
          .score = 20,
      }
  );
}

void add_tadpole(entt::registry &ecs, float start_x, float start_y) {
  std::vector<components::StepAnimationFrame> frames = {{4, 0}, {3, 0}, {4, 0}, {5, 0}};

  add_invader_entity(
      ecs,
      AddInvaderArgs{
          .start_x = start_x,
          .start_y = start_y,
          .frames = std::move(frames),
          .hitbox_offset_x = 4,
          .hitfox_offset_y = 2,
          .hitbox_w = 5,
          .hitbox_h = 9,
          .score = 10,
      }
  );
}

void add_invader_entities(entt::registry &ecs) {
  const float starting_x_pos = 2;
  float x_pos = starting_x_pos;
  float y_pos = 30;
  const float alien_width = 14;
  const float alien_height = 14;
  const float col_spacing = 2;
  const float row_spacing = -1.2;

  for (int i = 0; i < 11; i++) {
    add_octopus(ecs, x_pos, y_pos);
    x_pos += alien_width + col_spacing;
  }

  x_pos = starting_x_pos;
  y_pos += alien_height + row_spacing;
  for (int i = 0; i < 11; i++) {
    add_jellyfish(ecs, x_pos, y_pos);
    x_pos += alien_width + col_spacing;
  }

  x_pos = starting_x_pos;
  y_pos += alien_height + row_spacing;
  for (int i = 0; i < 11; i++) {
    add_crab(ecs, x_pos, y_pos);
    x_pos += alien_width + col_spacing;
  }

  x_pos = starting_x_pos;
  y_pos += alien_height + row_spacing;
  for (int i = 0; i < 11; i++) {
    add_tadpole(ecs, x_pos, y_pos);
    x_pos += alien_width + col_spacing;
  }

  x_pos = starting_x_pos;
  y_pos += alien_height + row_spacing;
  for (int i = 0; i < 11; i++) {
    add_tadpole(ecs, x_pos, y_pos);
    x_pos += alien_width + col_spacing;
  }

  ecs.ctx().insert_or_assign<components::singleton::InvaderOrchestration>(components::singleton::InvaderOrchestration{
      .move_right = true,
      .move_counter = 0,
      .shoot_counter = 0,
      .arp_idx = 0,
  });
}

void add_invader_projectile(entt::registry &ecs, float x, float y) {
  const auto entity = ecs.create();

  ecs.emplace<components::SoundOnDefeat>(entity, components::SoundOnDefeat{.audio = assets::Audio::AlienShot});

  ecs.emplace<components::ExplodeOnDefeat>(entity);

  ecs.emplace<components::InteractsOnCollide>(entity);

  ecs.emplace<components::DeleteOnGameOver>(entity);

  ecs.emplace<components::Position>(
      entity,
      components::Position{
          .x = x,
          .y = y,
          .w = 16,
          .h = 16,
          .z = 80,
      }
  );
  ecs.sort<components::Position>([](const auto &left, const auto &right) { return left.z < right.z; });

  ecs.emplace<components::Sprite>(
      entity,
      components::Sprite{
          .image = assets::Image::InvadersSpritesheet,
          .src_x = 80,
          .src_y = 32,
          .src_width = 16,
          .src_height = 15,
          .dst_width = 16,
          .dst_height = 15,
      }
  );

  std::vector<components::AnimationFrame> frames = {{5, 2}, {6, 2}};
  ecs.emplace<components::Animation>(
      entity,
      components::Animation{
          .playing = true,
          .play_reversed = false,
          .frames = std::move(frames),
          .cur_frame = 0,
          .ticks_per_frame = 40,
          .tick_counter = 0,
      }
  );

  ecs.emplace<components::Velocity>(
      entity,
      components::Velocity{
          .x = 0,
          .y = 1,
      }
  );

  ecs.emplace<components::Collision>(
      entity,
      components::Collision{
          .hitbox_offset_x = 7,
          .hitbox_offset_y = 7,
          .hitbox_w = 1,
          .hitbox_h = 3,
      }
  );

  components::DamageTypeSet susceptible_damage_types;
  susceptible_damage_types.set(components::damage_type_to_index(components::DamageType::Player));
  susceptible_damage_types.set(components::damage_type_to_index(components::DamageType::Player_Projectile));
  susceptible_damage_types.set(components::damage_type_to_index(components::DamageType::Fortress));
  ecs.emplace<components::Hitpoints>(
      entity,
      components::Hitpoints{
          .susceptible_to = susceptible_damage_types,
          .cur_hitpoints = 1,
      }
  );
  components::DamageTypeSet deal_damage_types;
  deal_damage_types.set(components::damage_type_to_index(components::DamageType::Alien_Projectile));
  ecs.emplace<components::DamageDealer>(
      entity,
      components::DamageDealer{
          .type = deal_damage_types,
          .amount = 1,
      }
  );
}

void add_explosion(entt::registry &ecs, float x, float y) {
  const auto entity = ecs.create();

  ecs.emplace<components::DeleteOnGameOver>(entity);

  ecs.emplace<components::Position>(
      entity,
      components::Position{
          .x = x,
          .y = y,
          .w = 16,
          .h = 16,
          .z = 200,
      }
  );
  ecs.emplace<components::Sprite>(
      entity,
      components::Sprite{
          .image = assets::Image::InvadersSpritesheet,
          .src_x = 48,
          .src_y = 16,
          .src_width = 16,
          .src_height = 16,
          .dst_width = 16,
          .dst_height = 16,
      }
  );
  std::vector<components::AnimationFrame> frames = {{3, 1}, {4, 1}, {5, 1}, {6, 1}};
  ecs.emplace<components::Animation>(
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
  ecs.emplace<components::Lifetime>(
      entity,
      components::Lifetime{
          .ticks = 20,
          .tick_counter = 0,
      }
  );
}

} // namespace invasion