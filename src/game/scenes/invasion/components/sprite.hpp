#pragma once

#include "game/assets/asset_enums.hpp"

namespace components {

struct Sprite {
    assets::Image image;
    float src_x;
    float src_y;
    float src_width;
    float src_height;
    float dst_width;
    float dst_height;
};

} // namespace components