#include "framework/sdl_asset_manager.hpp"
#include <SDL3/SDL.h>
#include <format>
#include <memory>
#include <stdexcept>
#include <string>

namespace framework {

struct SDLDeleter {
    void operator()(SDL_Texture *p) const {
      SDL_DestroyTexture(p);
    }

    void operator()(SDL_Surface *p) const {
      SDL_DestroySurface(p);
    }
};

SDLAssetManager::SDLAssetManager(SDL_Renderer *renderer)
    : renderer{renderer} {};

void SDLAssetManager::load_image_png(std::string src_id, std::string path) {
  auto png_surface = std::unique_ptr<SDL_Surface, SDLDeleter>(SDL_LoadPNG(path.c_str()));
  if (!png_surface.get()) {
    throw std::runtime_error(std::format("Failed to create PNG surface '{}': {}", path, SDL_GetError()));
  }

  auto png_texture
      = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, png_surface.get()), SDLDeleter());
  if (!png_texture.get()) {
    throw std::runtime_error(std::format("Failed to create PNG texture from surface '{}': {}", path, SDL_GetError()));
  }

  SDL_SetTextureScaleMode(png_texture.get(), SDL_SCALEMODE_PIXELART);

  textures.insert({src_id, png_texture});
}

std::shared_ptr<SDL_Texture> SDLAssetManager::get_texture(std::string src_id) const {
  return textures.at(src_id);
}

} // namespace framework