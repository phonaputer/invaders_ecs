#include "game/systems/player/game_over.hpp"
#include "core/point.hpp"
#include "framework/system.hpp"
#include "game/assets/asset_enums.hpp"
#include "game/components/deletable.hpp"
#include "game/components/delete_on_gameover.hpp"
#include "game/components/hitpoints.hpp"
#include "game/components/player_movement.hpp"
#include "game/components/position.hpp"
#include "game/components/singleton/hud_stats.hpp"
#include "game/constants.hpp"
#include "game/events/aliens_landed.hpp"
#include "game/events/defeated.hpp"
#include "game/events/pause.hpp"
#include "game/events/play_audio.hpp"
#include "game/events/stop_audio.hpp"
#include <entt.hpp>
#include <functional>

namespace systems::player {

GameOver::GameOver(
    std::function<void(entt::registry &, core::Point, unsigned int)> add_explosion,
    std::function<void(entt::registry &)> add_player,
    std::function<void(entt::registry &)> add_fortresses
)
    : add_explosion{add_explosion},
      add_player{add_player},
      add_fortresses{add_fortresses} {
}

void GameOver::execute(framework::ExecuteCtx &ctx) {
  if (pause_ongoing) {
    handle_ongoing_pause(ctx);
  } else {
    handle_defeat_if_any(ctx);
  }
}

void GameOver::handle_defeat_if_any(framework::ExecuteCtx &ctx) {
  if (ctx.events.get_singleton<events::AliensLanded>().has_value()) {
    handle_alien_landing(ctx);
    return;
  }

  auto player_entity = ctx.ecs.view<components::PlayerMovement>().front();
  if (!ctx.ecs.valid(player_entity)) {
    return;
  }

  for (const auto &event : ctx.events.get_all<events::Defeated>()) {
    if (event.entity == player_entity) {
      handle_player_defeat(ctx, player_entity);
      return;
    }
  }
}

void GameOver::handle_alien_landing(framework::ExecuteCtx &ctx) {
  ctx.events.set_singleton(events::Pause{.is_paused = true});

  auto stats = ctx.ecs.ctx().get<components::singleton::HUDStats>();
  stats.game_over = true;
  ctx.ecs.ctx().insert_or_assign(stats);

  game_is_over = true;
  pause_ongoing = true;
}

void GameOver::handle_player_defeat(framework::ExecuteCtx &ctx, entt::entity player_entity) {
  auto position = ctx.ecs.get<components::Position>(player_entity);

  add_explosion(ctx.ecs, {position.x, position.y}, PAUSE_TICKS);
  ctx.events.push_back_draw(events::PlayAudio{.audio = assets::Audio::PlayerExplosion});

  ctx.events.set_singleton(events::Pause{.is_paused = true});

  ctx.ecs.emplace_or_replace<components::Deleteable>(player_entity);

  auto stats = ctx.ecs.ctx().get<components::singleton::HUDStats>();
  stats.lives -= 1;

  if (stats.lives < 0) {
    stats.game_over = true;
    stats.lives = 0;
    game_is_over = true;
  }

  ctx.ecs.ctx().insert_or_assign(stats);

  pause_ongoing = true;
}

void GameOver::handle_ongoing_pause(framework::ExecuteCtx &ctx) {
  pause_counter++;
  if (pause_counter < PAUSE_TICKS) {
    return;
  }

  pause_counter = 0;
  pause_ongoing = false;

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
  ctx.events.push_back_draw(events::StopAudio{.audio = assets::Audio::PlayerExplosion});
}

void GameOver::delete_all_entities(framework::ExecuteCtx &ctx) {
  auto view = ctx.ecs.view<components::DeleteOnGameOver>();

  for (auto [entity] : view.each()) {
    ctx.ecs.emplace_or_replace<components::Deleteable>(entity);
  }
}

} // namespace systems::player