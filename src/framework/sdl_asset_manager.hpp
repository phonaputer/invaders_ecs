#pragma once

#include "framework/asset_loader.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <string>
#include <unordered_map>

namespace game {

class SDLAssetManager : public AssetLoader {
  private:
    // Renderer is cleaned up automatically by SDL, so it doesn't need to be a smart pointer.
    SDL_Renderer *renderer;
    std::unordered_map<std::string, std::shared_ptr<SDL_Texture>> textures;

  public:
    SDLAssetManager(SDL_Renderer *renderer);
    void load_image_png(std::string src_id, std::string path) override;
    std::shared_ptr<SDL_Texture> get_texture(std::string src_id) const;
};

} // namespace game