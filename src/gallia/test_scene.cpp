#include "gallia/test_scene.hpp"
#include "framework/game/scene.hpp"
#include "framework/game/scene_initialization_context.hpp"

namespace gallia {

void TestScene::initialize(game::SceneInitializationContext ctx) {
  ctx.assets.load_image_png("invaders_spritesheet", "./assets/space_invaders.png");
}

} // namespace gallia