#include "gallia/player_projectile.hpp"
#include "core/point.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "gallia/components/deletable.hpp"
#include "gallia/components/position.hpp"
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

} // namespace gallia