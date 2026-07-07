#include "gallia/invaders.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/game/constants.hpp"
#include "gallia/components/collision.hpp"
#include "gallia/components/deletable.hpp"
#include "gallia/components/invaders/is_invader.hpp"
#include "gallia/components/invaders/step_animation.hpp"
#include "gallia/components/position.hpp"
#include "gallia/components/sprite.hpp"
#include "gallia/components/starting_position.hpp"

namespace gallia {

struct AddInvaderArgs {
    float start_x;
    float start_y;
    std::vector<components::invaders::StepAnimationFrame> frames;
    float hitbox_offset_x;
    float hitfox_offset_y;
    float hitbox_w;
    float hitbox_h;
};

void add_invader_entity(ecs::ECS &ecs, AddInvaderArgs args) {
  auto entity = ecs.new_entity();

  ecs.components().set(entity, components::invaders::IsInvader{});

  ecs.components().set(
      entity,
      components::Position{
          .x = args.start_x,
          .y = args.start_y,
          .w = 16,
          .h = 16,
          .z = 99,
      }
  );

  ecs.components().set(
      entity,
      components::Collision{
          .hitbox_offset_x = args.hitbox_offset_x,
          .hitbox_offset_y = args.hitfox_offset_y,
          .hitbox_w = args.hitbox_w,
          .hitbox_h = args.hitbox_h,
      }
  );

  ecs.components().set(
      entity,
      components::StartingPosition{
          .x = game::WINDOW_WIDTH / 2 - 8,
          .y = 50,
      }
  );

  auto frame_zero = args.frames.at(0);

  ecs.components().set(
      entity,
      components::Sprite{
          .src_id = "invaders_spritesheet",
          .src_x = float(frame_zero.x) * 16,
          .src_y = float(frame_zero.y) * 16,
          .src_width = 16,
          .src_height = 16,
          .dst_width = 14,
          .dst_height = 14,
      }
  );

  ecs.components().set(
      entity,
      components::invaders::StepAnimation{
          .frames = std::move(args.frames),
          .cur_frame = 0,
      }
  );

  ecs.components().set(
      entity,
      components::Deleteable{
          .is_deleted = false,
      }
  );

  ecs.register_to_systems(entity);
}

void add_octopus(ecs::ECS &ecs, float start_x, float start_y) {
  std::vector<components::invaders::StepAnimationFrame> frames = {{6, 0}, {7, 0}};

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
      }
  );
}

void add_jellyfish(ecs::ECS &ecs, float start_x, float start_y) {
  std::vector<components::invaders::StepAnimationFrame> frames = {{1, 0}, {0, 0}, {1, 0}, {2, 0}};

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
      }
  );
}

void add_crab(ecs::ECS &ecs, float start_x, float start_y) {
  std::vector<components::invaders::StepAnimationFrame> frames = {{1, 1}, {0, 1}, {1, 1}, {2, 1}};

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
      }
  );
}

void add_tadpole(ecs::ECS &ecs, float start_x, float start_y) {
  std::vector<components::invaders::StepAnimationFrame> frames = {{4, 0}, {3, 0}, {4, 0}, {5, 0}};

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
      }
  );
}

void add_invader_entities(ecs::ECS &ecs) {
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
}

} // namespace gallia