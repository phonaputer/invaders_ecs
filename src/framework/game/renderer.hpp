#pragma once

#include <string>

namespace game {

struct DrawImageParams {
    std::string src_id;
    float src_x;
    float src_y;
    float src_width;
    float src_height;
    float dst_x;
    float dst_y;
    float dst_width;
    float dst_height;
};

class Renderer {
  public:
    virtual ~Renderer() = default;
    virtual void draw_image(const DrawImageParams &params) = 0;
};

} // namespace game