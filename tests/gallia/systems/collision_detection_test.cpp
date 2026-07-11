#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/default_ecs.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/event_broker.hpp"
#include "game/components/collision.hpp"
#include "game/components/position.hpp"
#include "game/events/collision_occurred.hpp"
#include "game/systems/collision_detection.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <string>

namespace events {
void PrintTo(const CollisionOccurred &col, std::ostream *os) {
  *os << "CollisionOccurred(" << col.who_am_i << "," << col.who_i_hit << ")";
}
} // namespace events

struct TestSetup {
    std::unique_ptr<ecs::ECS> ecs;
    systems::CollisionDetection system;
};

TestSetup setupTest() {
  auto ecs = std::make_unique<ecs::DefaultECS>(
      std::make_unique<ecs::ComponentManager>(), std::make_unique<ecs::EventBroker>()
  );

  return TestSetup{
      .ecs = std::move(ecs),
      .system = systems::CollisionDetection(),
  };
}

ecs::Entity createEntityWithHitbox(ecs::ECS &ecs, float x, float y, float w, float h) {
  auto e = ecs.new_entity();
  ecs.components().set(e, components::Position{.x = x, .y = y, .w = 1, .h = 1, .z = 100});
  ecs.components().set(
      e,
      components::Collision{
          .hitbox_offset_x = 0,
          .hitbox_offset_y = 0,
          .hitbox_w = w,
          .hitbox_h = h,
      }
  );

  return e;
}

void assertTotalHits(ecs::ECS &ecs, size_t expected) {
  auto num_hits = ecs.events().get_all<events::CollisionOccurred>().size();

  EXPECT_EQ(expected, num_hits / 2);
}

void assertHitEachOther(ecs::ECS &ecs, ecs::Entity left, ecs::Entity right) {
  auto hit_messages = ecs.events().get_all<events::CollisionOccurred>();

  EXPECT_THAT(hit_messages, testing::Contains(events::CollisionOccurred{.who_am_i = left, .who_i_hit = right}));
  EXPECT_THAT(hit_messages, testing::Contains(events::CollisionOccurred{.who_am_i = right, .who_i_hit = left}));
}

void assertHitNothing(ecs::ECS &ecs, ecs::Entity entity) {
  auto hit_messages = ecs.events().get_all<events::CollisionOccurred>();

  for (const auto &message : hit_messages) {
    EXPECT_FALSE(message.who_am_i == entity);
    EXPECT_FALSE(message.who_i_hit == entity);
  }
}

TEST(ComponentManager, ExecuteEntitiesExactlyOnTopOfEachOtherShouldMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1);
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1);
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHitEachOther(*setup.ecs, entity_one, entity_two);
}

TEST(ComponentManager, ExecuteWhenOneEntityWithinAnotherShouldMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1);
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 0, 0, 3, 3);
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHitEachOther(*setup.ecs, entity_one, entity_two);
}

TEST(ComponentManager, ExecuteWhenOneEntityThroughBottomOfAnotherShouldMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 3);
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 2, 2, 3, 3);
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHitEachOther(*setup.ecs, entity_one, entity_two);
}

TEST(ComponentManager, ExecuteWhenOneEntityThroughTopOfAnotherShouldMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 1, 3, 1, 2);
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 1, 1, 3, 3);
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHitEachOther(*setup.ecs, entity_one, entity_two);
}

TEST(ComponentManager, ExecuteWhenOneEntityThroughLeftOfAnotherShouldMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 0, 2, 2, 1);
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 1, 1, 3, 3);
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHitEachOther(*setup.ecs, entity_one, entity_two);
}

TEST(ComponentManager, ExecuteWhenOneEntityThroughRightOfAnotherShouldMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 3, 2, 2, 1);
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 1, 1, 3, 3);
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHitEachOther(*setup.ecs, entity_one, entity_two);
}

TEST(ComponentManager, ExecuteWhenEntitiesBarelyTouchingShouldMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 0, 0, 1, 1);
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 1, 1, 3, 3);
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHitEachOther(*setup.ecs, entity_one, entity_two);
}

TEST(ComponentManager, ExecuteBigEntityCrossingMultipleBucketsShouldStillRegisterCollisionsCorrectly) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 8, 8, 16, 16);
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 8, 8, 1, 1);
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());
  auto entity_three = createEntityWithHitbox(*setup.ecs, 23, 23, 1, 1);
  setup.system.add_entity_if_matches(entity_three, setup.ecs->components());
  auto entity_four = createEntityWithHitbox(*setup.ecs, 8, 23, 1, 1);
  setup.system.add_entity_if_matches(entity_four, setup.ecs->components());
  auto entity_five = createEntityWithHitbox(*setup.ecs, 23, 8, 1, 1);
  setup.system.add_entity_if_matches(entity_five, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertTotalHits(*setup.ecs, 4);
  assertHitEachOther(*setup.ecs, entity_one, entity_two);
  assertHitEachOther(*setup.ecs, entity_one, entity_three);
  assertHitEachOther(*setup.ecs, entity_one, entity_four);
  assertHitEachOther(*setup.ecs, entity_one, entity_five);
}

TEST(ComponentManager, ExecuteEntitiesOverlapAtNegativeCoordinatesShouldMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, -10, -10, 1, 1);
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, -10, -10, 1, 1);
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHitEachOther(*setup.ecs, entity_one, entity_two);
}

TEST(ComponentManager, ExecuteEntitiesTouchingInMultipleBucketsShouldOnlyRegisterOneHit) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 8, 8, 16, 16);
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 8, 8, 16, 16);
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertTotalHits(*setup.ecs, 1);
  assertHitEachOther(*setup.ecs, entity_one, entity_two);
}

TEST(ComponentManager, ExecuteWhenEntityAboveOtherShouldNOTMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 2, 5, 1, 1);
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 1, 1, 3, 3);
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHitNothing(*setup.ecs, entity_one);
  assertHitNothing(*setup.ecs, entity_two);
}

TEST(ComponentManager, ExecuteWhenEntityBelowOtherShouldNOTMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 2, -1, 1, 1);
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 1, 1, 3, 3);
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHitNothing(*setup.ecs, entity_one);
  assertHitNothing(*setup.ecs, entity_two);
}

TEST(ComponentManager, ExecuteWhenEntityRightOfOtherShouldNOTMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 5, 2, 1, 1);
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 1, 1, 3, 3);
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHitNothing(*setup.ecs, entity_one);
  assertHitNothing(*setup.ecs, entity_two);
}

TEST(ComponentManager, ExecuteWhenEntityLeftOfOtherShouldNOTMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, -1, 2, 1, 1);
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 1, 1, 3, 3);
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHitNothing(*setup.ecs, entity_one);
  assertHitNothing(*setup.ecs, entity_two);
}

TEST(ComponentManager, ExecuteMultipleEntitiesTouchingOneEntityShouldMarkAllOfThemAsHavingHitIt) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 1, 1, 3, 3);
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 3, 3, 1, 1);
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());
  auto entity_three = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1);
  setup.system.add_entity_if_matches(entity_three, setup.ecs->components());
  auto entity_four = createEntityWithHitbox(*setup.ecs, 1, 3, 1, 1);
  setup.system.add_entity_if_matches(entity_four, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertTotalHits(*setup.ecs, 3);
  assertHitEachOther(*setup.ecs, entity_one, entity_two);
  assertHitEachOther(*setup.ecs, entity_one, entity_three);
  assertHitEachOther(*setup.ecs, entity_one, entity_four);
}

TEST(ComponentManager, ExecuteMultipleHitsInOneFrameShouldMarkAllOfThem) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1);
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1);
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());
  auto entity_three = createEntityWithHitbox(*setup.ecs, 50, 50, 1, 1);
  setup.system.add_entity_if_matches(entity_three, setup.ecs->components());
  auto entity_four = createEntityWithHitbox(*setup.ecs, 100, 100, 1, 1);
  setup.system.add_entity_if_matches(entity_four, setup.ecs->components());
  auto entity_five = createEntityWithHitbox(*setup.ecs, 50, 50, 1, 1);
  setup.system.add_entity_if_matches(entity_five, setup.ecs->components());
  auto entity_six = createEntityWithHitbox(*setup.ecs, 3, 3, 3, 3);
  setup.system.add_entity_if_matches(entity_six, setup.ecs->components());
  auto entity_seven = createEntityWithHitbox(*setup.ecs, 4, 4, 1, 1);
  setup.system.add_entity_if_matches(entity_seven, setup.ecs->components());
  auto entity_eight = createEntityWithHitbox(*setup.ecs, 4, 4, 1, 1);
  setup.system.add_entity_if_matches(entity_eight, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertTotalHits(*setup.ecs, 5);
  assertHitEachOther(*setup.ecs, entity_one, entity_two);
  assertHitEachOther(*setup.ecs, entity_three, entity_five);
  assertHitNothing(*setup.ecs, entity_four);
  assertHitEachOther(*setup.ecs, entity_six, entity_seven);
  assertHitEachOther(*setup.ecs, entity_six, entity_eight);
  assertHitEachOther(*setup.ecs, entity_seven, entity_eight);
}

TEST(ComponentManager, RemoveEntityWhenEntityRemovedShouldNOTBeIncludedInCollisionChecks) {
  TestSetup setup = setupTest();

  auto entity_one = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1);
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  setup.system.remove_entity(entity_one);

  auto entity_two = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1);
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHitNothing(*setup.ecs, entity_one);
  assertHitNothing(*setup.ecs, entity_two);
}