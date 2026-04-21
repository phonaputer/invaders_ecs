#include "gallia/systems/animation.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "gallia/components/animation.hpp"
#include "gallia/components/sprite.hpp"
#include <set>

namespace systems {

void Animation::remove_entity(ecs::Entity entity) {
  entities.erase(entity);
}

void Animation::add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) {
  if (components.has<components::Animation>(entity) && components.has<components::Sprite>(entity)) {
    entities.insert(entity);
  }
}

void Animation::execute(ecs::ECS &ecs) {
  for (const auto &entity : entities) {
    auto animation = ecs.components().get<components::Animation>(entity);

    if (!animation.playing) {
      continue;
    }

    bool should_update_frame = false;

    animation.tick_counter++;
    if (animation.tick_counter >= animation.ticks_per_frame) {
      animation.tick_counter = 0;
      should_update_frame = true;

      animation.cur_frame++;
      if (animation.cur_frame >= animation.frames.size()) {
        animation.cur_frame = 0;
      }
    }

    ecs.components().set(entity, animation);

    if (should_update_frame) {
      auto sprite = ecs.components().get<components::Sprite>(entity);

      auto frame = animation.frames.at(animation.cur_frame);

      sprite.src_x = frame.x * sprite.src_width;
      sprite.src_y = frame.y * sprite.src_height;

      ecs.components().set(entity, sprite);
    }
  }
}

} // namespace systems