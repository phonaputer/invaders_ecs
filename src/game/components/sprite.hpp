#pragma once

#include <string>

namespace components {

struct Sprite {
    std::string src_id;
    float src_x;
    float src_y;
    float src_width;
    float src_height;
    float dst_width;
    float dst_height;
};

} // namespace components