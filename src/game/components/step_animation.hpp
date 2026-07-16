#pragma once

#include <vector>

namespace components {

struct StepAnimationFrame {
    int x;
    int y;
};

struct StepAnimation {
    std::vector<StepAnimationFrame> frames;
    unsigned int cur_frame;
};

} // namespace components