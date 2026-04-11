#include "framework/systems/rendering.hpp"
#include "framework/components/position.hpp"
#include "framework/components/sprite_static.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "framework/game/renderer.hpp"

namespace systems {

Rendering::Rendering(std::shared_ptr<game::Renderer> renderer)
    : renderer{renderer} {
}

void Rendering::remove_entity(ecs::Entity entity) {
  entities.erase(entity);
}

bool Rendering::matches_entity(ecs::Entity entity, ecs::ComponentManager &entity_components) {
  if (!entity_components.has<components::Position>(entity)) {
    return false;
  }

  if (entity_components.has<components::SpriteStatic>(entity)) {
    return true;
  }

  return false;
}

void Rendering::add_entity(ecs::Entity entity) {
  entities.insert(entity);
}

// TODO:
// * Handle non-static sprites
// * Handle z index
// * Unit tests
void Rendering::execute(ecs::ComponentManager &entity_components) {
  for (const auto &entity : entities) {
    auto position = entity_components.get<components::Position>(entity);
    auto sprite_static = entity_components.get<components::SpriteStatic>(entity);

    renderer->draw_image(
        game::DrawImageParams{
            .src_id = sprite_static.src_id,
            .src_x = sprite_static.src_x,
            .src_y = sprite_static.src_y,
            .src_width = sprite_static.src_width,
            .src_height = sprite_static.src_height,
            .dst_x = position.x,
            .dst_y = position.y,
            .dst_width = sprite_static.dst_width,
            .dst_height = sprite_static.dst_height,
        }
    );
  }
}

} // namespace systems