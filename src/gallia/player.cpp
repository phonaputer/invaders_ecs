#include "gallia/player.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/game/constants.hpp"
#include "gallia/components/animation.hpp"
#include "gallia/components/player_movement.hpp"
#include "gallia/components/player_shooting.hpp"
#include "gallia/components/position.hpp"
#include "gallia/components/sprite.hpp"
#include <vector>

namespace gallia {

// TODO animations for driving & muzzle flash
void add_player_entity(ecs::ECS &ecs) {
  auto entity = ecs.new_entity();

  ecs.components().set(
      entity,
      components::Position{
          .x = game::WINDOW_WIDTH / 2 - 8,
          .y = game::WINDOW_HEIGHT - 35,
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
          .ticks_per_frame = 5,
          .tick_counter = 0,
      }
  );
  ecs.components().set(
      entity,
      components::PlayerMovement{
          .x_speed = 1,
      }
  );
  ecs.components().set(
      entity,
      components::PlayerShooting{
          .ticks_per_shot = 7,
          .shot_clock = 0,
          .shot_offset_x = -2,
          .shot_offset_y = -2,
          .max_simultaneous_shots = 2,
      }
  );

  ecs.register_to_systems(entity);
}

} // namespace gallia