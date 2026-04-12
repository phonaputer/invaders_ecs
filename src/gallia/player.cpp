#include "gallia/player.hpp"
#include "framework/ecs/ecs.hpp"
#include "gallia/components/player_movement.hpp"
#include "gallia/components/position.hpp"
#include "gallia/components/sprite_static.hpp"

namespace gallia {

void add_player_entity(ecs::ECS &ecs) {
  auto entity = ecs.new_entity();

  ecs.components().set(
      entity,
      components::Position{
          .x = 50,
          .y = 50,
          .z = 100,
      }
  );
  ecs.components().set(
      entity,
      components::SpriteStatic{
          .src_id = "invaders_spritesheet",
          .src_x = 0,
          .src_y = 0,
          .src_width = 16,
          .src_height = 16,
          .dst_width = 16,
          .dst_height = 16,
      }
  );
  ecs.components().set(
      entity,
      components::PlayerMovement{
          .x_speed = 2,
          .width = 16,
      }
  );

  ecs.register_to_systems(entity);
}

} // namespace gallia