#include "framework/scene.hpp"
#include "game/assets/asset_enums.hpp"
#include "game/scenes/start_screen/scene.hpp"

namespace start_screen {

void Scene::initialize(framework::SceneInitializationContext ctx) {
  ctx.assets.load_image_png(
      assets::image_src_id(assets::Image::InvadersSpritesheet), assets::image_path(assets::Image::InvadersSpritesheet)
  );

  ctx.assets.load_audio_wav(assets::audio_sound_id(assets::Audio::MenuSelect), "./assets/menu_select.wav");
}

} // namespace start_screen