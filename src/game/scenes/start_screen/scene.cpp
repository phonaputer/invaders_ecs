#include "framework/scene.hpp"
#include "game/assets/asset_enums.hpp"
#include "game/scenes/start_screen/rendering_system.hpp"
#include "game/scenes/start_screen/scene.hpp"

namespace start_screen {

void Scene::initialize(framework::SceneInitializationContext ctx) {
  ctx.assets.load_image_png(
      assets::image_src_id(assets::Image::InvadersSpritesheet), assets::image_path(assets::Image::InvadersSpritesheet)
  );

  ctx.assets.load_audio_wav(assets::audio_sound_id(assets::Audio::MenuSelect), "./assets/menu_select.wav");

  ctx.systems.add_draw_system(std::make_unique<RenderingSystem>(ctx.scene_setter, ctx.audio_player, ctx.renderer));
}

} // namespace start_screen