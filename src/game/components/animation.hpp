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

    // Having a vector here is technically bad since the vector gets allocated
    // outside the contiguous memory of the ECS's vector of components, thereby
    // hurting CPU cache locality. But it's space invaders so who cares. I'm
    // just using ECS for kicks not for performance.
    std::vector<AnimationFrame> frames;

    unsigned int cur_frame;
    unsigned int ticks_per_frame;
    unsigned int tick_counter;
};

} // namespace components