#include "gallia/test_scene.hpp"
#include "framework/game/scene.hpp"
#include "framework/game/scene_initialization_context.hpp"
#include "gallia/player.hpp"

namespace gallia {

void TestScene::initialize(game::SceneInitializationContext ctx) {
  ctx.assets.load_image_png("invaders_spritesheet", "./assets/space_invaders.png");

  add_player_entity(ctx.ecs);
}

} // namespace gallia