#include "framework/renderer.hpp"
#include "framework/system.hpp"
#include "game/assets/asset_enums.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include "game/scenes/invasion/components/sprite.hpp"
#include "game/scenes/invasion/systems/rendering.hpp"
#include <entt.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

namespace testing::rendering_system {

class StubRenderer : public framework::Renderer {
  public:
    MOCK_METHOD(void, draw_image, (const framework::DrawImageParams &params), (override));
    MOCK_METHOD(void, draw_rect, (const framework::DrawRectParams &params), (override));
    MOCK_METHOD(void, draw_line, (const framework::DrawLineParams &params), (override));
};

struct TestSetup {
    entt::registry ecs;
    framework::EventBroker events;
    framework::PlayerInputManager player_input;
    std::shared_ptr<StubRenderer> renderer;
    systems::Rendering system;

    framework::ExecuteCtx ctx() {
      return framework::ExecuteCtx{
          .ecs = ecs,
          .events = events,
          .player_input = player_input,
      };
    }
};

TestSetup setupTest() {
  auto renderer = std::make_shared<StubRenderer>();

  return TestSetup{
      .ecs = entt::registry(),
      .events = framework::EventBroker(),
      .player_input = framework::PlayerInputManager(),
      .renderer = renderer,
      .system = systems::Rendering(*renderer),
  };
}

TEST(SystemRendering, ExecuteWithSpriteComponentEntitiesShouldRenderTheseEntities) {
  auto setup = setupTest();
  auto entity = setup.ecs.create();
  setup.ecs.emplace<components::Sprite>(
      entity,
      components::Sprite{
          .image = assets::Image::InvadersSpritesheet,
          .src_x = 5,
          .src_y = 6,
          .src_width = 7,
          .src_height = 8,
          .dst_width = 9,
          .dst_height = 10,
      }
  );
  setup.ecs.emplace<components::Position>(entity, components::Position{.x = 1, .y = 2, .w = 3, .h = 4, .z = 100});
  auto entity_two = setup.ecs.create();
  setup.ecs.emplace<components::Sprite>(
      entity_two,
      components::Sprite{
          .image = assets::Image::InvadersSpritesheet,
          .src_x = 51,
          .src_y = 61,
          .src_width = 71,
          .src_height = 81,
          .dst_width = 91,
          .dst_height = 101,
      }
  );
  setup.ecs.emplace<components::Position>(
      entity_two, components::Position{.x = 11, .y = 21, .w = 31, .h = 41, .z = 101}
  );
  auto ctx = setup.ctx();

  auto expected_draw = framework::DrawImageParams{
      .src_id = assets::image_src_id(assets::Image::InvadersSpritesheet),
      .src_x = 5,
      .src_y = 6,
      .src_width = 7,
      .src_height = 8,
      .dst_x = 1,
      .dst_y = 2,
      .dst_width = 9,
      .dst_height = 10,
  };
  EXPECT_CALL(*setup.renderer, draw_image(expected_draw)).Times(1);
  auto expected_draw_two = framework::DrawImageParams{
      .src_id = assets::image_src_id(assets::Image::InvadersSpritesheet),
      .src_x = 51,
      .src_y = 61,
      .src_width = 71,
      .src_height = 81,
      .dst_x = 11,
      .dst_y = 21,
      .dst_width = 91,
      .dst_height = 101,
  };
  EXPECT_CALL(*setup.renderer, draw_image(expected_draw_two)).Times(1);

  setup.system.execute(ctx);
}

} // namespace testing::rendering_system