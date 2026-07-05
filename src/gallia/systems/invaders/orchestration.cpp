#include "gallia/systems/invaders/orchestration.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "framework/game/constants.hpp"
#include "framework/game/player_input_manager.hpp"
#include "gallia/components/invaders/enabled.hpp"
#include "gallia/components/invaders/step_animation.hpp"
#include "gallia/components/position.hpp"
#include "gallia/components/sprite.hpp"
#include "gallia/components/starting_position.hpp"
#include <set>

namespace systems::invaders {

void Orchestration::remove_entity(ecs::Entity entity) {
  entities.erase(entity);
}

void Orchestration::add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) {
  if (components.has<components::invaders::Enabled>(entity) && components.has<components::Position>(entity)
      && components.has<components::StartingPosition>(entity)
      && components.has<components::invaders::StepAnimation>(entity) && components.has<components::Sprite>(entity)) {
    entities.insert(entity);
  }
}

void Orchestration::execute(ecs::ECS &ecs) {
  bool should_move = false;
  tick_counter++;
  if (tick_counter > BASE_TICKS_PER_MOVE) {
    should_move = true;
    tick_counter = 0;
  }

  if (!should_move) {
    return;
  }

  bool hit_wall = false;
  std::set<ecs::Entity> active_entities;

  for (const auto &entity : entities) {
    auto enabled = ecs.components().get<components::invaders::Enabled>(entity);
    if (!enabled.is_enabled) {
      continue;
    }

    active_entities.insert(entity);

    auto position = ecs.components().get<components::Position>(entity);
    if (position.x + position.w >= game::WINDOW_WIDTH - 16 || position.x < 16) {
      hit_wall = true;
    }
  }

  if (active_entities.size() < 1) {
    return; // TODO rerack
  }

  if (hit_wall) {
    move_right = !move_right;
  }

  for (const auto &entity : active_entities) {
    auto position = ecs.components().get<components::Position>(entity);

    if (hit_wall) {
      position.y += Y_SPEED;
      // TODO hit floor check
    }

    auto x_diff = X_SPEED;
    if (!move_right) {
      x_diff = -x_diff;
    }

    position.x += x_diff;

    ecs.components().set(entity, position);

    auto animation = ecs.components().get<components::invaders::StepAnimation>(entity);

    animation.cur_frame++;
    if (animation.cur_frame >= animation.frames.size()) {
      animation.cur_frame = 0;
    }

    ecs.components().set(entity, animation);

    auto sprite = ecs.components().get<components::Sprite>(entity);

    auto frame = animation.frames.at(animation.cur_frame);

    sprite.src_x = frame.x * sprite.src_width;
    sprite.src_y = frame.y * sprite.src_height;

    ecs.components().set(entity, sprite);
  }
}

} // namespace systems::invaders