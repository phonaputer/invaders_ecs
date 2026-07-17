#include "game/scenes/start_screen/rendering_system.hpp"
#include "framework/constants.hpp"
#include "framework/player_input.hpp"
#include "framework/renderer.hpp"
#include "framework/system.hpp"
#include "game/assets/asset_enums.hpp"
#include "game/scenes/invasion/scene.hpp"
#include "game/util/text_renderer.hpp"
#include <format>
#include <memory>

namespace start_screen {

RenderingSystem::RenderingSystem(
    framework::SceneSetter &scene_setter, framework::AudioPlayer &audio_player, framework::Renderer &renderer
)
    : scene_setter{scene_setter},
      audio_player{audio_player},
      renderer{renderer},
      text_renderer{renderer} {
}

void RenderingSystem::execute(framework::ExecuteCtx &ctx) {
  if (game_start_engaged) {
    game_start_tick_counter++;
    if (game_start_tick_counter % GAME_START_BLINK_TICKS == 0) {
      game_start_blink = !game_start_blink;
    }

    if (game_start_tick_counter > GAME_START_TOTAL_TICKS) {
      scene_setter.set_scene(std::make_unique<invasion::Scene>());
    }
  }

  if (!game_start_engaged && ctx.player_input.is_initiated(framework::PlayerInput::FIRE)) {
    game_start_engaged = true;
    audio_player.play_sound(assets::audio_sound_id(assets::Audio::MenuSelect));
  }

  text_renderer.render_text_centered(60, "personal space invaders");

  renderer.draw_image(
      framework::DrawImageParams{
          .src_id = assets::image_src_id(assets::Image::InvadersSpritesheet),
          .src_x = 64,
          .src_y = 0,
          .src_width = 16,
          .src_height = 16,
          .dst_x = 60,
          .dst_y = 85,
          .dst_width = 16,
          .dst_height = 16
      }
  );
  text_renderer.render_text_centered(90, " - 10 points");

  renderer.draw_image(
      framework::DrawImageParams{
          .src_id = assets::image_src_id(assets::Image::InvadersSpritesheet),
          .src_x = 16,
          .src_y = 16,
          .src_width = 16,
          .src_height = 16,
          .dst_x = 60,
          .dst_y = 100,
          .dst_width = 16,
          .dst_height = 16
      }
  );
  text_renderer.render_text_centered(105, " - 20 points");

  renderer.draw_image(
      framework::DrawImageParams{
          .src_id = assets::image_src_id(assets::Image::InvadersSpritesheet),
          .src_x = 16,
          .src_y = 0,
          .src_width = 16,
          .src_height = 16,
          .dst_x = 60,
          .dst_y = 115,
          .dst_width = 16,
          .dst_height = 16
      }
  );
  text_renderer.render_text_centered(120, " - 30 points");

  renderer.draw_image(
      framework::DrawImageParams{
          .src_id = assets::image_src_id(assets::Image::InvadersSpritesheet),
          .src_x = 96,
          .src_y = 0,
          .src_width = 16,
          .src_height = 16,
          .dst_x = 60,
          .dst_y = 130,
          .dst_width = 16,
          .dst_height = 16
      }
  );
  text_renderer.render_text_centered(135, " - 40 points");

  text_renderer.render_text_centered(165, "<a> and <d> to move");
  text_renderer.render_text_centered(175, "<space> to shoot");
  text_renderer.render_text_centered(185, "<esc> to pause");

  if (game_start_engaged && game_start_blink) {
    text_renderer.render_text_centered(215, "press <space> to begin");
  } else if (!game_start_engaged) {
    text_renderer.render_text_centered(215, "press <space> to begin");
  }
}

} // namespace start_screen