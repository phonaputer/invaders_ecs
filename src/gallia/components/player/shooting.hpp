#pragma once

namespace components::player {

struct Shooting {
    int ticks_per_shot;
    int shot_clock;
    float shot_offset_x;
    float shot_offset_y;
    unsigned int max_simultaneous_shots;
};

} // namespace components