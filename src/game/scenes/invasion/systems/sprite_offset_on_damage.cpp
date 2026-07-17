#include "game/scenes/invasion/systems/sprite_offset_on_damage.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/sprite.hpp"
#include "game/scenes/invasion/components/sprite_offset_on_damage.hpp"
#include "game/scenes/invasion/events/took_damage.hpp"

namespace systems {

void SpriteOffsetOnDamage::execute(framework::ExecuteCtx &ctx) {
  for (const auto &event : ctx.events.get_all<events::TookDamage>()) {
    if (!ctx.ecs.all_of<components::Sprite, components::SpriteOffsetOnDamage>(event.entity)) {
      continue;
    }

    auto sprite = ctx.ecs.get<components::Sprite>(event.entity);
    auto offset = ctx.ecs.get<components::SpriteOffsetOnDamage>(event.entity);

    sprite.src_x += offset.offset_x * event.amount;
    sprite.src_y += offset.offset_y * event.amount;

    ctx.ecs.replace<components::Sprite>(event.entity, sprite);
  }
}

} // namespace systems