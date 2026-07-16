#include "game/systems/player/defeat.hpp"
#include "core/point.hpp"
#include "framework/system.hpp"
#include "game/components/deletable.hpp"
#include "game/components/delete_on_gameover.hpp"
#include "game/components/hitpoints.hpp"
#include "game/components/player_movement.hpp"
#include "game/components/position.hpp"
#include "game/components/singleton/hud_stats.hpp"
#include "game/constants.hpp"
#include "game/events/defeated.hpp"
#include "game/events/pause.hpp"
#include <entt.hpp>
#include <functional>

namespace systems::player {

Defeat::Defeat(
    std::function<void(entt::registry &, core::Point, unsigned int)> add_explosion,
    std::function<void(entt::registry &)> add_player,
    std::function<void(entt::registry &)> add_fortresses
)
    : add_explosion{add_explosion},
      add_player{add_player},
      add_fortresses{add_fortresses} {
}

void Defeat::execute(framework::ExecuteCtx &ctx) {
  if (defeat_pause_ongoing) {
    handle_ongoing_pause(ctx);
  } else {
    handle_defeat_if_any(ctx);
  }
}

void Defeat::handle_defeat_if_any(framework::ExecuteCtx &ctx) {
  auto player_entity = ctx.ecs.view<components::PlayerMovement>().front();
  if (!ctx.ecs.valid(player_entity)) {
    return;
  }

  for (const auto &event : ctx.events.get_all<events::Defeated>()) {
    if (event.entity == player_entity) {
      handle_defeat(ctx, player_entity);
      return;
    }
  }
}

void Defeat::handle_defeat(framework::ExecuteCtx &ctx, entt::entity player_entity) {
  auto position = ctx.ecs.get<components::Position>(player_entity);

  add_explosion(ctx.ecs, {position.x, position.y}, DEFEAT_PAUSE_TICKS);

  ctx.events.set_singleton(
      events::Pause{
          .is_paused = true,
      }
  );

  ctx.ecs.emplace_or_replace<components::Deleteable>(player_entity);

  auto stats = ctx.ecs.ctx().get<components::singleton::HUDStats>();
  stats.lives -= 1;

  if (stats.lives < 0) {
    stats.game_over = true;
    stats.lives = 0;
    game_is_over = true;
  }

  ctx.ecs.ctx().insert_or_assign(stats);

  defeat_pause_ongoing = true;
}

void Defeat::handle_ongoing_pause(framework::ExecuteCtx &ctx) {
  defeat_pause_counter++;
  if (defeat_pause_counter < DEFEAT_PAUSE_TICKS) {
    return;
  }

  defeat_pause_counter = 0;
  defeat_pause_ongoing = false;

  ctx.events.set_singleton(
      events::Pause{
          .is_paused = false,
      }
  );

  if (game_is_over) {
    game_is_over = false;

    auto stats = ctx.ecs.ctx().get<components::singleton::HUDStats>();
    stats.score = 0;
    stats.lives = game::STARTING_LIVES;
    stats.game_over = false;
    ctx.ecs.ctx().insert_or_assign(stats);

    delete_all_entities(ctx);

    add_fortresses(ctx.ecs);
  }

  add_player(ctx.ecs);
}

void Defeat::delete_all_entities(framework::ExecuteCtx &ctx) {
  auto view = ctx.ecs.view<components::DeleteOnGameOver>();

  for (auto [entity] : view.each()) {
    ctx.ecs.emplace_or_replace<components::Deleteable>(entity);
  }
}

} // namespace systems::player