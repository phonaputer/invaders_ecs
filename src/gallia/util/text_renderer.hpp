#pragma once

#include "framework/game/renderer.hpp"
#include <string>

namespace util {

class TextRenderer {
  private:
    static constexpr float CHARACTER_SRC_WIDTH = 8;
    static constexpr float CHARACTER_SRC_HEIGHT = 8;

    static constexpr float CHARACTER_DRAW_WIDTH = 8;
    static constexpr float CHARACTER_DRAW_HEIGHT = 8;

    const std::string IMAGE_SRC = "invaders_spritesheet";

    game::Renderer &renderer;

  public:
    static constexpr float CHARACTER_X_SPACING = 6;

    TextRenderer(game::Renderer &renderer);
    void render_text(float x, float y, std::string text) const;
    void render_text_centered(float y, std::string text) const;
};

} // namespace util