#include "game/systems/sprite_offset_on_damage.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "game/components/sprite.hpp"
#include "game/components/sprite_offset_on_damage.hpp"
#include "game/events/took_damage.hpp"
#include <set>

namespace systems {

void SpriteOffsetOnDamage::remove_entity(ecs::Entity entity) {
  entities.erase(entity);
}

void SpriteOffsetOnDamage::add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) {
  if (components.has<components::Sprite>(entity) && components.has<components::SpriteOffsetOnDamage>(entity)) {
    entities.insert(entity);
  }
}

void SpriteOffsetOnDamage::execute(ecs::ECS &ecs) {
  for (const auto &event : ecs.events().get_all<events::TookDamage>()) {
    if (entities.contains(event.entity)) {
      auto sprite = ecs.components().get<components::Sprite>(event.entity);
      auto offset = ecs.components().get<components::SpriteOffsetOnDamage>(event.entity);

      sprite.src_x += offset.offset_x * event.amount;
      sprite.src_y += offset.offset_y * event.amount;

      ecs.components().set(event.entity, sprite);
    }
  }
}

} // namespace systems