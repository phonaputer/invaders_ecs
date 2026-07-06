#include "gallia/systems/invaders/orchestration.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
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
  if (!should_move_this_tick()) {
    return;
  }

  bool should_move_down = false;
  active_entities_this_tick.clear();

  for (const auto &entity : entities) {
    auto enabled = ecs.components().get<components::invaders::Enabled>(entity);
    if (!enabled.is_enabled) {
      continue;
    }

    active_entities_this_tick.insert(entity);
    should_move_down = should_move_down || did_hit_wall(ecs, entity);
  }

  if (active_entities_this_tick.size() < 1) {
    return; // TODO rerack
  }

  if (should_move_down) {
    move_right = !move_right;
  }

  for (const auto &entity : active_entities_this_tick) {
    move(ecs, entity, should_move_down);
    animate(ecs, entity);
  }
}

bool Orchestration::should_move_this_tick() {
  tick_counter++;
  if (tick_counter > BASE_TICKS_PER_MOVE) {
    tick_counter = 0;
    return true;
  }

  return false;
}

bool Orchestration::did_hit_wall(ecs::ECS &ecs, ecs::Entity entity) {
  auto position = ecs.components().get<components::Position>(entity);

  auto delta_x = X_SPEED;
  if (!move_right) {
    delta_x = -delta_x;
  }

  auto new_x = position.x + delta_x;

  if (new_x + position.w >= RIGHT_MOVEMENT_BOUNDARY || new_x < LEFT_MOVEMENT_BOUNDARY) {
    return true;
  }

  return false;
}

void Orchestration::move(ecs::ECS &ecs, ecs::Entity entity, bool move_down) {
  auto position = ecs.components().get<components::Position>(entity);

  auto delta_x = X_SPEED;
  if (!move_right) {
    delta_x = -delta_x;
  }

  if (move_down) {
    position.y += Y_SPEED;
    // TODO hit floor check
  }

  position.x += delta_x;

  ecs.components().set(entity, position);
}

void Orchestration::animate(ecs::ECS &ecs, ecs::Entity entity) {
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

} // namespace systems::invaders