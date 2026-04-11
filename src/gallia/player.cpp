#include "gallia/player.hpp"
#include "framework/components/position.hpp"
#include "framework/components/sprite_static.hpp"
#include "framework/ecs/ecs.hpp"

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
          .dst_width = 50,
          .dst_height = 50,
      }
  );

  ecs.register_to_systems(entity);
}

} // namespace gallia