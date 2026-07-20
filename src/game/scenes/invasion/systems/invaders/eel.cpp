#include "game/scenes/invasion/systems/invaders/eel.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/singleton/eel_invader.hpp"
#include "game/scenes/invasion/components/singleton/paused.hpp"
#include <random>

namespace systems::invaders {

std::uniform_int_distribution<int> NEXT_EEL_TICKS_DISTRIBUTION(600, 1200);

Eel::Eel(std::function<void(entt::registry &, std::mt19937 &)> spawn_eel, unsigned int rand_seed)
    : spawn_eel{spawn_eel},
      rand_gen{rand_seed} {
}

void Eel::execute(framework::ExecuteCtx &ctx) {
  if (ctx.ecs.ctx().get<components::singleton::Paused>().paused) {
    return;
  }

  // Doing the first initialization here instead of the scene file because the
  // distribution is stored here.
  if (!ctx.ecs.ctx().contains<components::singleton::EelInvader>()) {
    ctx.ecs.ctx().insert_or_assign(
        components::singleton::EelInvader{
            .ticks_until_next_spawn = NEXT_EEL_TICKS_DISTRIBUTION(rand_gen),
            .tick_counter = 0,
        }
    );
    return;
  }

  auto state = ctx.ecs.ctx().get<components::singleton::EelInvader>();

  state.tick_counter++;

  if (state.tick_counter > state.ticks_until_next_spawn) {
    spawn_eel(ctx.ecs, rand_gen);

    state.ticks_until_next_spawn = NEXT_EEL_TICKS_DISTRIBUTION(rand_gen);
    state.tick_counter = 0;
  }

  ctx.ecs.ctx().insert_or_assign(state);
}

} // namespace systems::invaders