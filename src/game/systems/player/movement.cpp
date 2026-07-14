#include "game/systems/player/movement.hpp"
#include "framework/constants.hpp"
#include "framework/player_input.hpp"
#include "framework/player_input_manager.hpp"
#include "framework/system.hpp"
#include "game/components/animation.hpp"
#include "game/components/player/movement.hpp"
#include "game/components/position.hpp"

namespace systems::player {

void Movement::execute(framework::ExecuteCtx &ctx) {
  auto view = ctx.ecs.view<components::player::Movement, components::Position, components::Animation>();

  for (auto [entity, movement, position, animation] : view.each()) {
    if (ctx.player_input.is_engaged(framework::PlayerInput::LEFT)
        == ctx.player_input.is_engaged(framework::PlayerInput::RIGHT)) {
      animation.playing = false;
      continue;
    }

    if (ctx.player_input.is_engaged(framework::PlayerInput::LEFT)) {
      auto new_x = position.x - movement.x_speed;
      if (new_x < 0) {
        new_x = 0;
      }

      if (new_x != position.x) {
        animation.playing = true;
        animation.play_reversed = true;
      } else {
        animation.playing = false;
      }

      position.x = new_x;
    }

    if (ctx.player_input.is_engaged(framework::PlayerInput::RIGHT)) {
      auto new_x = position.x + movement.x_speed;
      if (new_x + position.w > framework::WINDOW_WIDTH) {
        new_x = framework::WINDOW_WIDTH - position.w;
      }

      if (new_x != position.x) {
        animation.playing = true;
        animation.play_reversed = false;
      } else {
        animation.playing = false;
      }

      position.x = new_x;
    }
  }
}

} // namespace systems::player