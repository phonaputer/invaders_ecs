#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/default_ecs.hpp"
#include "framework/ecs/ecs.hpp"
#include "gallia/components/collision.hpp"
#include "gallia/components/position.hpp"
#include "gallia/systems/collision_detection.hpp"
#include <gtest/gtest.h>
#include <memory>
#include <string>

struct TestSetup {
    std::unique_ptr<ecs::ECS> ecs;
    systems::CollisionDetection system;
};

TestSetup setupTest() {
  auto ecs = std::make_unique<ecs::DefaultECS>(std::make_unique<ecs::ComponentManager>());

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
          .hit_something_this_tick = false,
          .who_i_hit = 0,
      }
  );

  return e;
}

void assertHitEachOther(ecs::ECS &ecs, ecs::Entity left, ecs::Entity right) {
  auto left_collision = ecs.components().get<components::Collision>(left);
  auto right_collision = ecs.components().get<components::Collision>(right);

  EXPECT_TRUE(left_collision.hit_something_this_tick);
  EXPECT_EQ(right, left_collision.who_i_hit);

  EXPECT_TRUE(right_collision.hit_something_this_tick);
  EXPECT_EQ(left, right_collision.who_i_hit);
}

void assertHitNothing(ecs::ECS &ecs, ecs::Entity entity) {
  auto collision = ecs.components().get<components::Collision>(entity);

  EXPECT_FALSE(collision.hit_something_this_tick);
  EXPECT_EQ(0, collision.who_i_hit);
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

TEST(ComponentManager, ExecuteMultipleEntitiesTouchingOneEntityShouldMarkOnlyOneOfThemAsHavingHitIt) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 1, 1, 3, 3);
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 3, 3, 1, 1);
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());
  auto entity_three = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1);
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());
  auto entity_four = createEntityWithHitbox(*setup.ecs, 1, 3, 1, 1);
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHitEachOther(*setup.ecs, entity_one, entity_two);
  assertHitNothing(*setup.ecs, entity_three);
  assertHitNothing(*setup.ecs, entity_four);
}

TEST(ComponentManager, ExecuteMultipleHitsInOneFrameShouldMarkAllOfThem) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1);
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1);
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());
  auto entity_three = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1);
  setup.system.add_entity_if_matches(entity_three, setup.ecs->components());
  auto entity_threehalf = createEntityWithHitbox(*setup.ecs, 100, 100, 1, 1);
  setup.system.add_entity_if_matches(entity_threehalf, setup.ecs->components());
  auto entity_four = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1);
  setup.system.add_entity_if_matches(entity_four, setup.ecs->components());
  auto entity_five = createEntityWithHitbox(*setup.ecs, 3, 3, 3, 3);
  setup.system.add_entity_if_matches(entity_five, setup.ecs->components());
  auto entity_six = createEntityWithHitbox(*setup.ecs, 4, 4, 1, 1);
  setup.system.add_entity_if_matches(entity_six, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHitEachOther(*setup.ecs, entity_one, entity_two);
  assertHitEachOther(*setup.ecs, entity_three, entity_four);
  assertHitEachOther(*setup.ecs, entity_five, entity_six);
  assertHitNothing(*setup.ecs, entity_threehalf);
}

TEST(ComponentManager, ExecuteEntityHitSomethingOnPreviousTickButNotThisTickShouldNOTBeMarkedAsHittingAnything) {
  TestSetup setup = setupTest();

  auto entity_one = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1);
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto e1_col = setup.ecs->components().get<components::Collision>(entity_one);
  e1_col.hit_something_this_tick = true;
  e1_col.who_i_hit = 100;
  setup.ecs->components().set(entity_one, e1_col);

  setup.system.execute(*setup.ecs);

  assertHitNothing(*setup.ecs, entity_one);
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