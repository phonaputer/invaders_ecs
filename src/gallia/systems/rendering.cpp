#include "gallia/systems/rendering.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "framework/game/renderer.hpp"
#include "gallia/components/position.hpp"
#include "gallia/components/sprite.hpp"

namespace systems {

Rendering::Rendering(game::Renderer &renderer)
    : renderer{renderer} {
}

void Rendering::remove_entity(ecs::Entity entity) {
  entities.erase(entity);
}

void Rendering::add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &entity_components) {
  if (!entity_components.has<components::Position>(entity)) {
    return;
  }

  if (entity_components.has<components::Sprite>(entity)) {
    entities.insert(entity);
  }
}

// TODO:
// * Handle non-static sprites
// * Handle z index
// * Unit tests
void Rendering::execute(ecs::ECS &ecs) {
  for (const auto &entity : entities) {
    auto position = ecs.components().get<components::Position>(entity);
    auto sprite = ecs.components().get<components::Sprite>(entity);

    renderer.draw_image(
        game::DrawImageParams{
            .src_id = sprite.src_id,
            .src_x = sprite.src_x,
            .src_y = sprite.src_y,
            .src_width = sprite.src_width,
            .src_height = sprite.src_height,
            .dst_x = position.x,
            .dst_y = position.y,
            .dst_width = sprite.dst_width,
            .dst_height = sprite.dst_height,
        }
    );
  }
}

} // namespace systems