#include "game/scenes/invasion/systems/animation.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/animation.hpp"
#include "game/scenes/invasion/components/singleton/paused.hpp"
#include "game/scenes/invasion/components/sprite.hpp"

namespace systems {

void Animation::execute(framework::ExecuteCtx &ctx) {
  auto pause = ctx.ecs.ctx().get<components::singleton::Paused>();

  auto view = ctx.ecs.view<components::Animation, components::Sprite>();

  for (auto [entity, animation, sprite] : view.each()) {
    if (!animation.playing) {
      continue;
    }

    if (pause.paused && animation.pausable) {
      continue;
    }

    bool should_update_frame = false;

    animation.tick_counter++;
    if (animation.tick_counter >= animation.ticks_per_frame) {
      animation.tick_counter = 0;
      should_update_frame = true;

      if (animation.play_reversed) {
        if (animation.cur_frame == 0) {
          animation.cur_frame = animation.frames.size() - 1;
        } else {
          animation.cur_frame--;
        }
      } else {
        animation.cur_frame++;
        if (animation.cur_frame >= animation.frames.size()) {
          animation.cur_frame = 0;
        }
      }
    }

    ctx.ecs.replace<components::Animation>(entity, animation);

    if (should_update_frame) {
      auto frame = animation.frames.at(animation.cur_frame);

      sprite.src_x = frame.x * sprite.src_width;
      sprite.src_y = frame.y * sprite.src_height;
    }
  }
}

} // namespace systems