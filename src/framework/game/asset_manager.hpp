#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <string>
#include <unordered_map>

namespace game {

// TODO would be nice to have a way to create a version of this independent from SDL
class AssetManager {
  private:
    // Renderer is cleaned up automatically by SDL, so it doesn't need to be a smart pointer.
    SDL_Renderer *renderer;
    std::unordered_map<std::string, std::shared_ptr<SDL_Texture>> textures;

  public:
    AssetManager(SDL_Renderer *renderer);

    void load_png_texture(std::string src_id, std::string path);
    std::shared_ptr<SDL_Texture> get_texture(std::string src_id) const;
};

} // namespace game