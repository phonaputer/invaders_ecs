#include "game/scenes/invasion/systems/player/shooting.hpp"
#include "core/point.hpp"
#include "framework/player_input.hpp"
#include "framework/player_input_manager.hpp"
#include "framework/system.hpp"
#include "game/assets/asset_enums.hpp"
#include "game/scenes/invasion/components/deletable.hpp"
#include "game/scenes/invasion/components/player_shooting.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include "game/scenes/invasion/events/play_audio.hpp"
#include <entt.hpp>
#include <functional>

namespace systems::player {

Shooting::Shooting(
    std::function<entt::entity(entt::registry &ecs, core::Point)> add_projectile,
    std::function<void(entt::registry &, entt::entity)> add_muzzle_flash
)
    : add_projectile{add_projectile},
      add_muzzle_flash{add_muzzle_flash} {
}

void Shooting::execute(framework::ExecuteCtx &ctx) {
  auto view = ctx.ecs.view<components::PlayerShooting, components::Position>();

  for (auto [entity, shooting, position] : view.each()) {
    std::erase_if(active_projectiles, [&ctx](entt::entity projectile) {
      return !ctx.ecs.all_of<components::Position>(projectile);
    });

    bool fire = false;

    if (ctx.player_input.is_engaged(framework::PlayerInput::FIRE)
        && active_projectiles.size() < shooting.max_simultaneous_shots
        && shooting.shot_clock >= shooting.ticks_per_shot) {
      fire = true;
      shooting.shot_clock = 0;
    } else {
      shooting.shot_clock++;
    }

    if (fire) {
      active_projectiles.insert(add_projectile(
          ctx.ecs,
          core::Point{
              .x = position.x + shooting.shot_offset_x,
              .y = position.y - shooting.shot_offset_y,
          }
      ));
      add_muzzle_flash(ctx.ecs, entity);
      ctx.events.push_back_draw(events::PlayAudio{.audio = assets::Audio::PlayerShot});
    }
  }
}

} // namespace systems::player