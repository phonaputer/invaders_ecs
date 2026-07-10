#include "gallia/systems/invaders/orchestration.hpp"
#include "core/point.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "framework/game/player_input_manager.hpp"
#include "gallia/components/invaders/step_animation.hpp"
#include "gallia/components/position.hpp"
#include "gallia/components/sprite.hpp"
#include "gallia/components/starting_position.hpp"
#include <random>
#include <utility>
#include <vector>

namespace systems::invaders {

Orchestration::Orchestration(std::function<void(ecs::ECS &, core::Point)> add_projectile, unsigned int rand_seed)
    : add_projectile{add_projectile},
      rand_gen{rand_seed} {
}

void Orchestration::remove_entity(ecs::Entity entity) {
  auto to_remove = std::find(entities.begin(), entities.end(), entity);
  if (to_remove != entities.end()) {
    std::swap(*to_remove, entities.back());
    entities.pop_back();
  }
}

void Orchestration::add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) {
  if (components.has<components::Position>(entity) && components.has<components::StartingPosition>(entity)
      && components.has<components::invaders::StepAnimation>(entity) && components.has<components::Sprite>(entity)) {
    entities.push_back(entity);
  }
}

void Orchestration::execute(ecs::ECS &ecs) {
  if (should_shoot_this_tick()) {
    random_alien_shoot(ecs);
  }

  if (!should_move_this_tick()) {
    return;
  }

  bool should_move_down = false;
  for (const auto &entity : entities) {
    should_move_down = should_move_down || did_hit_wall(ecs, entity);
  }

  if (should_move_down) {
    move_right = !move_right;
  }

  for (const auto &entity : entities) {
    move(ecs, entity, should_move_down);
    animate(ecs, entity);
  }
}

bool Orchestration::should_shoot_this_tick() {
  shoot_counter++;
  if (shoot_counter < TICKS_PER_SHOOT_CHANCE) {
    return false;
  }

  shoot_counter = 0;
  std::uniform_int_distribution<int> hit_roll(1, ALIEN_SHOOT_CHANCE);

  return hit_roll(rand_gen) == 1;
}

void Orchestration::random_alien_shoot(ecs::ECS &ecs) {
  if (entities.size() < 1) {
    return;
  }

  std::uniform_int_distribution<int> alien_selection(0, entities.size() - 1);
  auto selected_entity = entities.at(alien_selection(rand_gen));

  auto position = ecs.components().get<components::Position>(selected_entity);
  add_projectile(ecs, {position.x, position.y});
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