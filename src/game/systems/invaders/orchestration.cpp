#include "game/systems/invaders/orchestration.hpp"
#include "core/point.hpp"
#include "framework/player_input_manager.hpp"
#include "framework/system.hpp"
#include "game/components/invader.hpp"
#include "game/components/position.hpp"
#include "game/components/sprite.hpp"
#include "game/components/step_animation.hpp"
#include "game/events/pause.hpp"
#include <entt.hpp>
#include <iterator>
#include <random>
#include <utility>

namespace systems::invaders {

Orchestration::Orchestration(
    std::function<void(entt::registry &, core::Point)> add_projectile,
    std::function<void(entt::registry &)> rerack_aliens,
    unsigned int rand_seed
)
    : add_projectile{add_projectile},
      rerack_aliens{rerack_aliens},
      rand_gen{rand_seed} {
}

void Orchestration::execute(framework::ExecuteCtx &ctx) {
  const auto pause = ctx.events.get_singleton<events::Pause>();
  if (pause.has_value()) {
    paused = pause.value().is_paused;
  }

  if (paused) {
    return;
  }

  auto view = ctx.ecs.view<components::Invader>();

  if (view.empty()) {
    rerack_aliens(ctx.ecs);
    move_right = true;
  }

  if (should_shoot_this_tick()) {
    random_alien_shoot(ctx.ecs);
  }

  if (!should_move_this_tick()) {
    return;
  }

  bool should_move_down = did_hit_wall(ctx.ecs);

  if (should_move_down) {
    move_right = !move_right;
  }

  move(ctx.ecs, should_move_down);
  animate(ctx.ecs);
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

void Orchestration::random_alien_shoot(entt::registry &ecs) {
  auto view = ecs.view<components::Invader>();
  if (view.empty()) {
    return;
  }

  std::uniform_int_distribution<int> alien_selection(0, view.size() - 1);
  auto it = view.begin();
  std::advance(it, alien_selection(rand_gen));
  auto selected_entity = *it;

  auto position = ecs.get<components::Position>(selected_entity);
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

bool Orchestration::did_hit_wall(entt::registry &ecs) {
  auto view = ecs.view<components::Invader, const components::Position>();

  for (auto [entity, position] : view.each()) {
    auto delta_x = X_SPEED;
    if (!move_right) {
      delta_x = -delta_x;
    }

    auto new_x = position.x + delta_x;

    if (new_x + position.w >= RIGHT_MOVEMENT_BOUNDARY || new_x < LEFT_MOVEMENT_BOUNDARY) {
      return true;
    }
  }

  return false;
}

void Orchestration::move(entt::registry &ecs, bool move_down) {
  auto view = ecs.view<components::Invader, components::Position>();

  for (auto [entity, position] : view.each()) {
    auto delta_x = X_SPEED;
    if (!move_right) {
      delta_x = -delta_x;
    }

    if (move_down) {
      position.y += Y_SPEED;
    }

    position.x += delta_x;
  }
}

void Orchestration::animate(entt::registry &ecs) {
  auto view = ecs.view<components::StepAnimation, components::Sprite>();

  for (auto [entity, animation, sprite] : view.each()) {
    animation.cur_frame++;
    if (animation.cur_frame >= animation.frames.size()) {
      animation.cur_frame = 0;
    }

    auto frame = animation.frames.at(animation.cur_frame);

    sprite.src_x = frame.x * sprite.src_width;
    sprite.src_y = frame.y * sprite.src_height;
  }
}

} // namespace systems::invaders