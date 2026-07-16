#pragma once

#include <string>

namespace components {

struct Sprite {
    // TODO consider alternatives to string.
    // Technically string isn't great for performance since it'll get allocated
    // outside the contiguous array of sprite structs which is bad for CPU
    // cache.
    // Doesn't matter for a game like this though.
    std::string src_id;
    float src_x;
    float src_y;
    float src_width;
    float src_height;
    float dst_width;
    float dst_height;
};

} // namespace components