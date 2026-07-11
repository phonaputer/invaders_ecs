#include "game/systems/debug/hitbox_rendering.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "framework/renderer.hpp"
#include "game/components/collision.hpp"
#include "game/components/position.hpp"

namespace systems::debug {

HitboxRendering::HitboxRendering(game::Renderer &renderer)
    : renderer{renderer} {
}

void HitboxRendering::remove_entity(ecs::Entity entity) {
  entities.erase(entity);
}

void HitboxRendering::add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &entity_components) {
  if (entity_components.has<components::Position>(entity) && entity_components.has<components::Collision>(entity)) {
    entities.insert(entity);
  }
}

void HitboxRendering::execute(ecs::ECS &ecs) {
  for (const auto &entity : entities) {
    auto position = ecs.components().get<components::Position>(entity);
    auto collision = ecs.components().get<components::Collision>(entity);

    renderer.draw_rect(
        game::DrawRectParams{
            .x = position.x + collision.hitbox_offset_x,
            .y = position.y + collision.hitbox_offset_y,
            .width = collision.hitbox_w,
            .height = collision.hitbox_h,
            .r = 255,
            .g = 0,
            .b = 255,
        }
    );
  }
}

} // namespace systems::debug