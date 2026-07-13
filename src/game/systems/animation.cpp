#include "game/components/animation.hpp"
#include "game/components/sprite.hpp"
#include <flecs.h>

namespace systems {

flecs::system animation(flecs::world world) {
  world.system<components::Animation, components::Sprite>("Animation")
      .each([](components::Animation &animation, components::Sprite &sprite) {
        if (!animation.playing) {
          return;
        }

        bool should_update_frame = false;

        auto cur_ticks = ++animation.tick_counter;
        if (cur_ticks >= animation.ticks_per_frame) {
          animation.tick_counter = 0;
          should_update_frame = true;

          if (animation.play_reversed) {
            if (animation.cur_frame = 0) {
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

        if (should_update_frame) {
          auto frame = animation.frames.at(animation.cur_frame);

          sprite.src_x = frame.x * sprite.src_width;
          sprite.src_y = frame.y * sprite.src_height;
        }
      });
}

} // namespace systems