#pragma once

#include <string>

namespace components {

struct SpriteStatic {
    std::string src_id;
    float src_width;
    float src_height;
    float dst_width;
    float dst_height;
};

} // namespace components