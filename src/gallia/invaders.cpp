#include "gallia/invaders.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/game/constants.hpp"
#include "gallia/components/invaders/enabled.hpp"
#include "gallia/components/invaders/step_animation.hpp"
#include "gallia/components/position.hpp"
#include "gallia/components/sprite.hpp"
#include "gallia/components/starting_position.hpp"

namespace gallia {

void add_invader_entities(ecs::ECS &ecs) {
  auto entity = ecs.new_entity();

  ecs.components().set(
      entity,
      components::Position{
          .x = game::WINDOW_WIDTH / 2 - 8,
          .y = 50,
          .w = 16,
          .h = 16,
          .z = 99,
      }
  );
  ecs.components().set(
      entity,
      components::StartingPosition{
          .x = game::WINDOW_WIDTH / 2 - 8,
          .y = 50,
      }
  );
  ecs.components().set(
      entity,
      components::Sprite{
          .src_id = "invaders_spritesheet",
          .src_x = 16,
          .src_y = 0,
          .src_width = 16,
          .src_height = 16,
          .dst_width = 16,
          .dst_height = 16,
      }
  );
  std::vector<components::invaders::StepAnimationFrame> frames = {{1, 0}, {2, 0}, {1, 0}, {0, 0}};
  ecs.components().set(
      entity,
      components::invaders::StepAnimation{
          .frames = std::move(frames),
          .cur_frame = 0,
      }
  );
  ecs.components().set(
      entity,
      components::invaders::Enabled{
          .is_enabled = true,
      }
  );

  ecs.register_to_systems(entity);
}

} // namespace gallia