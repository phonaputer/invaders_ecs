#pragma once

#include <vector>

namespace components {

struct AnimationFrame {
    int x;
    int y;
};

struct Animation {
    bool playing;
    bool play_reversed;
    std::vector<AnimationFrame> frames;
    unsigned int cur_frame;
    unsigned int ticks_per_frame;
    unsigned int tick_counter;
};

} // namespace components