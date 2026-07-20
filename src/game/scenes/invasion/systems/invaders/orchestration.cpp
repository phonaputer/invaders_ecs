#include "game/scenes/invasion/systems/invaders/orchestration.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/invader.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include "game/scenes/invasion/components/singleton/invader_orchestration.hpp"
#include "game/scenes/invasion/components/singleton/paused.hpp"
#include "game/scenes/invasion/components/sprite.hpp"
#include "game/scenes/invasion/components/step_animation.hpp"
#include "game/scenes/invasion/constants.hpp"
#include "game/scenes/invasion/events/play_audio.hpp"
#include <entt.hpp>
#include <iterator>
#include <random>

namespace systems::invaders {

Orchestration::Orchestration(
    std::function<void(entt::registry &, float, float)> add_projectile,
    std::function<void(entt::registry &)> rerack_aliens,
    unsigned int rand_seed
)
    : add_projectile{add_projectile},
      rerack_aliens{rerack_aliens},
      rand_gen{rand_seed} {
}

void Orchestration::execute(framework::ExecuteCtx &ctx) {
  const auto pause = ctx.ecs.ctx().get<components::singleton::Paused>();
  if (pause.paused) {
    return;
  }

  auto view = ctx.ecs.view<components::Invader>();
  int num_aliens = view.size();

  if (num_aliens < 1) {
    rerack_aliens(ctx.ecs);
    return;
  }

  if (should_shoot_this_tick(ctx.ecs)) {
    random_alien_shoot(ctx.ecs);
  }

  if (!should_move_this_tick(ctx.ecs, num_aliens)) {
    return;
  }

  bool should_move_down = did_hit_wall(ctx.ecs);

  if (should_move_down) {
    auto orchestration = ctx.ecs.ctx().get<components::singleton::InvaderOrchestration>();
    orchestration.move_right = !orchestration.move_right;
    ctx.ecs.ctx().insert_or_assign(orchestration);
  }

  move(ctx, should_move_down, num_aliens);
  animate(ctx.ecs);
}

bool Orchestration::should_shoot_this_tick(entt::registry &ecs) {
  auto orchestration = ecs.ctx().get<components::singleton::InvaderOrchestration>();

  orchestration.shoot_counter++;
  if (orchestration.shoot_counter < TICKS_PER_SHOOT_CHANCE) {
    ecs.ctx().insert_or_assign(orchestration);
    return false;
  }

  orchestration.shoot_counter = 0;
  ecs.ctx().insert_or_assign(orchestration);

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

  std::uniform_int_distribution<int> shoot_jitter(0, SHOOT_X_JITTER * 2);
  auto jitter = shoot_jitter(rand_gen) - SHOOT_X_JITTER;

  auto position = ecs.get<components::Position>(selected_entity);
  add_projectile(ecs, position.x + jitter, position.y);
}

bool Orchestration::should_move_this_tick(entt::registry &ecs, int num_aliens) {
  auto orchestration = ecs.ctx().get<components::singleton::InvaderOrchestration>();

  orchestration.move_counter++;

  if (orchestration.move_counter > BASE_TICKS_PER_MOVE + num_aliens) {
    orchestration.move_counter = 0;
    ecs.ctx().insert_or_assign(orchestration);
    return true;
  }

  ecs.ctx().insert_or_assign(orchestration);

  return false;
}

bool Orchestration::did_hit_wall(entt::registry &ecs) {
  auto orchestration = ecs.ctx().get<components::singleton::InvaderOrchestration>();

  auto view = ecs.view<components::Invader, const components::Position>();

  for (auto [entity, position] : view.each()) {
    auto delta_x = X_SPEED;
    if (!orchestration.move_right) {
      delta_x = -delta_x;
    }

    auto new_x = position.x + delta_x;

    if (new_x + position.w >= RIGHT_MOVEMENT_BOUNDARY || new_x < LEFT_MOVEMENT_BOUNDARY) {
      return true;
    }
  }

  return false;
}

void Orchestration::move(framework::ExecuteCtx &ctx, bool move_down, int num_aliens) {
  auto orchestration = ctx.ecs.ctx().get<components::singleton::InvaderOrchestration>();

  auto x_speed = X_SPEED;
  if (num_aliens == 1) {
    x_speed = LAST_MAN_STANDING_X_SPEED;
  }

  auto delta_x = x_speed;
  if (!orchestration.move_right) {
    delta_x = -delta_x;
  }

  auto delta_y = 0;
  if (move_down) {
    delta_y = Y_SPEED;
  }

  auto view = ctx.ecs.view<components::Invader, components::Position>();

  for (auto [entity, position] : view.each()) {
    position.y += delta_y;
    position.x += delta_x;
  }

  ctx.events.push_back_draw(events::PlayAudio{.audio = ARP_SOUNDS.at(orchestration.arp_idx)});
  orchestration.arp_idx++;
  if (orchestration.arp_idx >= ARP_SOUNDS.size()) {
    orchestration.arp_idx = 0;
  }
  ctx.ecs.ctx().insert_or_assign(orchestration);
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