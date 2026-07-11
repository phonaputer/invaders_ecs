#pragma once

#include <string>

namespace game {

class AssetLoader {
  public:
    virtual ~AssetLoader() = default;
    virtual void load_image_png(std::string src_id, std::string path) = 0;
};

} // namespace game