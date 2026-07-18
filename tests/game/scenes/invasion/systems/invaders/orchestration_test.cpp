#include "framework/event_broker.hpp"
#include "game/assets/asset_enums.hpp"
#include "game/scenes/invasion/components/invader.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include "game/scenes/invasion/components/singleton/invader_orchestration.hpp"
#include "game/scenes/invasion/components/singleton/paused.hpp"
#include "game/scenes/invasion/components/sprite.hpp"
#include "game/scenes/invasion/components/step_animation.hpp"
#include "game/scenes/invasion/events/play_audio.hpp"
#include "game/scenes/invasion/systems/invaders/orchestration.hpp"
#include <entt.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

namespace events {
void PrintTo(const PlayAudio &e, std::ostream *os) {
  *os << "PlayAudio(" << assets::audio_sound_id(e.audio) << ")";
}
} // namespace events

namespace testing::invader_orchestration_system {

entt::entity add_invader(entt::registry &ecs, float x, float y) {
  auto entity = ecs.create();
  ecs.emplace<components::Invader>(entity);
  ecs.emplace<components::Position>(
      entity,
      components::Position{
          .x = x,
          .y = y,
          .w = 10,
          .h = 11,
          .z = 12,
      }
  );

  return entity;
}

struct TestSetup {
    entt::registry ecs;
    framework::EventBroker events;
    framework::PlayerInputManager player_input;

    ::testing::MockFunction<void(entt::registry &, float, float)> add_projectile;
    ::testing::MockFunction<void(entt::registry &)> rerack_aliens;

    std::unique_ptr<systems::invaders::Orchestration> system(unsigned int rand_seed = 1) {
      return std::make_unique<systems::invaders::Orchestration>(
          add_projectile.AsStdFunction(), rerack_aliens.AsStdFunction(), rand_seed
      );
    }

    framework::ExecuteCtx ctx() {
      return framework::ExecuteCtx{
          .ecs = ecs,
          .events = events,
          .player_input = player_input,
      };
    }
};

std::unique_ptr<TestSetup> setupTest() {
  auto setup = std::make_unique<TestSetup>();

  setup->ecs.ctx().insert_or_assign(components::singleton::Paused{.paused = false});

  return setup;
}

TEST(SystemInvaderOrchestration, ExecuteWhenGameIsPausedShouldDoNothing) {
  auto setup = setupTest();
  setup->ecs.ctx().insert_or_assign(components::singleton::Paused{.paused = true});
  EXPECT_CALL(setup->rerack_aliens, Call(::testing::_)).Times(0);
  EXPECT_CALL(setup->add_projectile, Call(::testing::_, ::testing::_, ::testing::_)).Times(0);
  auto ctx = setup->ctx();

  setup->system()->execute(ctx);
}

TEST(SystemInvaderOrchestration, ExecuteWhenZeroInvadersExistShouldRerack) {
  auto setup = setupTest();
  EXPECT_CALL(setup->rerack_aliens, Call(::testing::_)).Times(1);
  EXPECT_CALL(setup->add_projectile, Call(::testing::_, ::testing::_, ::testing::_)).Times(0);
  auto ctx = setup->ctx();

  setup->system()->execute(ctx);
}

TEST(SystemInvaderOrchestration, ExecuteWhenNeitherShootNorMoveShouldDoNothing) {
  auto setup = setupTest();
  setup->ecs.ctx().insert_or_assign(
      components::singleton::InvaderOrchestration{
          .move_right = true,
          .move_counter = 0,
          .shoot_counter = 0,
          .arp_idx = 0,
      }
  );
  auto invader = add_invader(setup->ecs, 100, 101);
  EXPECT_CALL(setup->rerack_aliens, Call(::testing::_)).Times(0);
  EXPECT_CALL(setup->add_projectile, Call(::testing::_, ::testing::_, ::testing::_)).Times(0);
  auto ctx = setup->ctx();

  setup->system()->execute(ctx);

  auto position = setup->ecs.get<components::Position>(invader);
  EXPECT_EQ(100.0, position.x);
  EXPECT_EQ(101.0, position.y);
}

TEST(SystemInvaderOrchestration, ExecuteWhenMoveShouldShiftRight) {
  auto setup = setupTest();
  setup->ecs.ctx().insert_or_assign(
      components::singleton::InvaderOrchestration{
          .move_right = true,
          .move_counter = 1000,
          .shoot_counter = 0,
          .arp_idx = 0,
      }
  );
  auto invader = add_invader(setup->ecs, 100, 101);
  auto invader_two = add_invader(setup->ecs, 200, 201);
  auto ctx = setup->ctx();

  setup->system()->execute(ctx);

  auto position = setup->ecs.get<components::Position>(invader);
  EXPECT_EQ(108.0, position.x);
  EXPECT_EQ(101.0, position.y);
  auto position_two = setup->ecs.get<components::Position>(invader_two);
  EXPECT_EQ(208.0, position_two.x);
  EXPECT_EQ(201.0, position_two.y);
}

TEST(SystemInvaderOrchestration, ExecuteWhenMoveAndSingleInvaderRemainsShouldShiftRightAtHighSpeed) {
  auto setup = setupTest();
  setup->ecs.ctx().insert_or_assign(
      components::singleton::InvaderOrchestration{
          .move_right = true,
          .move_counter = 1000,
          .shoot_counter = 0,
          .arp_idx = 0,
      }
  );
  auto invader = add_invader(setup->ecs, 100, 101);
  auto ctx = setup->ctx();

  setup->system()->execute(ctx);

  auto position = setup->ecs.get<components::Position>(invader);
  EXPECT_EQ(116.0, position.x);
  EXPECT_EQ(101.0, position.y);
}

TEST(SystemInvaderOrchestration, ExecuteWhenMoveDirectionIsLeftShouldShiftLeft) {
  auto setup = setupTest();
  setup->ecs.ctx().insert_or_assign(
      components::singleton::InvaderOrchestration{
          .move_right = false,
          .move_counter = 1000,
          .shoot_counter = 0,
          .arp_idx = 0,
      }
  );
  auto invader = add_invader(setup->ecs, 100, 101);
  auto invader_two = add_invader(setup->ecs, 200, 201);
  auto ctx = setup->ctx();

  setup->system()->execute(ctx);

  auto position = setup->ecs.get<components::Position>(invader);
  EXPECT_EQ(92.0, position.x);
  EXPECT_EQ(101.0, position.y);
  auto position_two = setup->ecs.get<components::Position>(invader_two);
  EXPECT_EQ(192.0, position_two.x);
  EXPECT_EQ(201.0, position_two.y);
}

TEST(SystemInvaderOrchestration, ExecuteWhenMoveDirectionIsLeftAndSingleInvaderRemainsShouldShiftLeftAtHighSpeed) {
  auto setup = setupTest();
  setup->ecs.ctx().insert_or_assign(
      components::singleton::InvaderOrchestration{
          .move_right = false,
          .move_counter = 1000,
          .shoot_counter = 0,
          .arp_idx = 0,
      }
  );
  auto invader = add_invader(setup->ecs, 100, 101);
  auto ctx = setup->ctx();

  setup->system()->execute(ctx);

  auto position = setup->ecs.get<components::Position>(invader);
  EXPECT_EQ(84.0, position.x);
  EXPECT_EQ(101.0, position.y);
}

TEST(SystemInvaderOrchestration, ExecuteWhenMoveAndPastRightBoundaryShouldSwitchDirectionAndGoDown) {
  auto setup = setupTest();
  setup->ecs.ctx().insert_or_assign(
      components::singleton::InvaderOrchestration{
          .move_right = true,
          .move_counter = 1000,
          .shoot_counter = 0,
          .arp_idx = 0,
      }
  );
  auto invader = add_invader(setup->ecs, 100, 101);
  auto invader_two = add_invader(setup->ecs, 250, 201);
  auto ctx = setup->ctx();

  setup->system()->execute(ctx);

  auto position = setup->ecs.get<components::Position>(invader);
  EXPECT_EQ(92.0, position.x);
  EXPECT_EQ(113.0, position.y);
  auto position_two = setup->ecs.get<components::Position>(invader_two);
  EXPECT_EQ(242.0, position_two.x);
  EXPECT_EQ(213.0, position_two.y);
  auto orchestration = setup->ecs.ctx().get<components::singleton::InvaderOrchestration>();
  EXPECT_FALSE(orchestration.move_right);
}

TEST(SystemInvaderOrchestration, ExecuteWhenMoveAndPastLeftBoundaryShouldSwitchDirectionAndGoDown) {
  auto setup = setupTest();
  setup->ecs.ctx().insert_or_assign(
      components::singleton::InvaderOrchestration{
          .move_right = false,
          .move_counter = 1000,
          .shoot_counter = 0,
          .arp_idx = 0,
      }
  );
  auto invader = add_invader(setup->ecs, 100, 101);
  auto invader_two = add_invader(setup->ecs, 0, 201);
  auto ctx = setup->ctx();

  setup->system()->execute(ctx);

  auto position = setup->ecs.get<components::Position>(invader);
  EXPECT_EQ(108.0, position.x);
  EXPECT_EQ(113.0, position.y);
  auto position_two = setup->ecs.get<components::Position>(invader_two);
  EXPECT_EQ(8.0, position_two.x);
  EXPECT_EQ(213.0, position_two.y);
  auto orchestration = setup->ecs.ctx().get<components::singleton::InvaderOrchestration>();
  EXPECT_TRUE(orchestration.move_right);
}

TEST(SystemInvaderOrchestration, ExecuteWhenMoveShouldPlayArpAndIncrementNote) {
  auto setup = setupTest();
  setup->ecs.ctx().insert_or_assign(
      components::singleton::InvaderOrchestration{
          .move_right = true,
          .move_counter = 1000,
          .shoot_counter = 0,
          .arp_idx = 0,
      }
  );
  add_invader(setup->ecs, 100, 101);
  auto ctx = setup->ctx();

  setup->system()->execute(ctx);

  auto orchestration = setup->ecs.ctx().get<components::singleton::InvaderOrchestration>();
  EXPECT_EQ(1, orchestration.arp_idx);
  auto audio_events = setup->events.get_all_draw<events::PlayAudio>();
  EXPECT_EQ(1, audio_events.size());
  EXPECT_EQ(assets::Audio::Arp1, audio_events.at(0).audio);
}

TEST(SystemInvaderOrchestration, ExecuteWhenMoveAndArpIsOnLastNoteShouldWrapAroundToFirstNote) {
  auto setup = setupTest();
  setup->ecs.ctx().insert_or_assign(
      components::singleton::InvaderOrchestration{
          .move_right = true,
          .move_counter = 1000,
          .shoot_counter = 0,
          .arp_idx = 3,
      }
  );
  add_invader(setup->ecs, 100, 101);
  auto ctx = setup->ctx();

  setup->system()->execute(ctx);

  auto orchestration = setup->ecs.ctx().get<components::singleton::InvaderOrchestration>();
  EXPECT_EQ(0, orchestration.arp_idx);
  auto audio_events = setup->events.get_all_draw<events::PlayAudio>();
  EXPECT_EQ(1, audio_events.size());
  EXPECT_EQ(assets::Audio::Arp4, audio_events.at(0).audio);
}

} // namespace testing::invader_orchestration_system