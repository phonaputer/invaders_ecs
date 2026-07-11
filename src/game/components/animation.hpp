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

    // Having a vector here feels kinda lazy since the vector gets allocated
    // outside the contiguous memory of the ECS's vector of components but it's
    // space invaders so who cares.
    //
    // TODO May fix this later.
    std::vector<AnimationFrame> frames;

    unsigned int cur_frame;
    unsigned int ticks_per_frame;
    unsigned int tick_counter;
};

} // namespace components