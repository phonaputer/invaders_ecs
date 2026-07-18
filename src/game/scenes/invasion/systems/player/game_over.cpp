#include "game/scenes/invasion/systems/player/game_over.hpp"
#include "framework/system.hpp"
#include "game/assets/asset_enums.hpp"
#include "game/scenes/invasion/components/deletable.hpp"
#include "game/scenes/invasion/components/delete_on_gameover.hpp"
#include "game/scenes/invasion/components/hitpoints.hpp"
#include "game/scenes/invasion/components/player_movement.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include "game/scenes/invasion/components/singleton/hud_stats.hpp"
#include "game/scenes/invasion/components/singleton/paused.hpp"
#include "game/scenes/invasion/constants.hpp"
#include "game/scenes/invasion/events/aliens_landed.hpp"
#include "game/scenes/invasion/events/defeated.hpp"
#include "game/scenes/invasion/events/play_audio.hpp"
#include "game/scenes/invasion/events/stop_audio.hpp"
#include <entt.hpp>
#include <functional>

namespace systems::player {

GameOver::GameOver(
    std::function<void(entt::registry &, float, float, unsigned int)> add_explosion,
    std::function<void(entt::registry &)> add_player,
    std::function<void(entt::registry &)> add_fortresses
)
    : add_explosion{add_explosion},
      add_player{add_player},
      add_fortresses{add_fortresses} {
}

void GameOver::execute(framework::ExecuteCtx &ctx) {
  const auto pause = ctx.ecs.ctx().get<components::singleton::Paused>();

  if (pause.paused) {
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
  ctx.ecs.ctx().insert_or_assign(components::singleton::Paused{.paused = true});

  auto stats = ctx.ecs.ctx().get<components::singleton::HUDStats>();
  stats.game_over = true;
  ctx.ecs.ctx().insert_or_assign(stats);

  game_is_over = true;
}

void GameOver::handle_player_defeat(framework::ExecuteCtx &ctx, entt::entity player_entity) {
  auto position = ctx.ecs.get<components::Position>(player_entity);

  add_explosion(ctx.ecs, position.x, position.y, PAUSE_TICKS);
  ctx.events.push_back_draw(events::PlayAudio{.audio = assets::Audio::PlayerExplosion});

  ctx.ecs.ctx().insert_or_assign(components::singleton::Paused{.paused = true});

  ctx.ecs.emplace_or_replace<components::Deleteable>(player_entity);

  auto stats = ctx.ecs.ctx().get<components::singleton::HUDStats>();
  stats.lives -= 1;

  if (stats.lives < 0) {
    stats.game_over = true;
    stats.lives = 0;
    game_is_over = true;
  }

  ctx.ecs.ctx().insert_or_assign(stats);
}

void GameOver::handle_ongoing_pause(framework::ExecuteCtx &ctx) {
  pause_counter++;
  if (pause_counter < PAUSE_TICKS) {
    return;
  }

  pause_counter = 0;

  ctx.ecs.ctx().insert_or_assign(components::singleton::Paused{.paused = false});

  if (game_is_over) {
    game_is_over = false;

    auto stats = ctx.ecs.ctx().get<components::singleton::HUDStats>();
    stats.score = 0;
    stats.lives = invasion::STARTING_LIVES;
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