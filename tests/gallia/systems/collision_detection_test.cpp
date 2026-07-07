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

ecs::Entity createEntityWithHitbox(ecs::ECS &ecs, float x, float y, float w, float h, std::string type) {
  auto e = ecs.new_entity();
  ecs.components().set(e, components::Position{.x = x, .y = y, .w = 1, .h = 1, .z = 100});
  ecs.components().set(
      e,
      components::Collision{
          .active = true,
          .type = type,
          .hitbox_offset_x = 0,
          .hitbox_offset_y = 0,
          .hitbox_w = w,
          .hitbox_h = h,
          .hit_something_this_tick = false,
          .type_of_what_i_hit = "",
      }
  );

  return e;
}

void assertHit(ecs::ECS &ecs, ecs::Entity entity, std::string what_i_hit) {
  auto collision = ecs.components().get<components::Collision>(entity);

  EXPECT_TRUE(collision.hit_something_this_tick);
  EXPECT_EQ(what_i_hit, collision.type_of_what_i_hit);
}

void assertHitNothing(ecs::ECS &ecs, ecs::Entity entity) {
  auto collision = ecs.components().get<components::Collision>(entity);

  EXPECT_FALSE(collision.hit_something_this_tick);
}

TEST(ComponentManager, ExecuteEntitiesExactlyOnTopOfEachOtherShouldMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1, "thing1");
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1, "thing2");
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHit(*setup.ecs, entity_one, "thing2");
  assertHit(*setup.ecs, entity_two, "thing1");
}

TEST(ComponentManager, ExecuteWhenOneEntityWithinAnotherShouldMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1, "thing1");
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 0, 0, 3, 3, "thing2");
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHit(*setup.ecs, entity_one, "thing2");
  assertHit(*setup.ecs, entity_two, "thing1");
}

TEST(ComponentManager, ExecuteWhenOneEntityThroughBottomOfAnotherShouldMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 3, "thing1");
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 2, 2, 3, 3, "thing2");
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHit(*setup.ecs, entity_one, "thing2");
  assertHit(*setup.ecs, entity_two, "thing1");
}

TEST(ComponentManager, ExecuteWhenOneEntityThroughTopOfAnotherShouldMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 1, 3, 1, 2, "thing1");
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 1, 1, 3, 3, "thing2");
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHit(*setup.ecs, entity_one, "thing2");
  assertHit(*setup.ecs, entity_two, "thing1");
}

TEST(ComponentManager, ExecuteWhenOneEntityThroughLeftOfAnotherShouldMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 0, 2, 2, 1, "thing1");
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 1, 1, 3, 3, "thing2");
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHit(*setup.ecs, entity_one, "thing2");
  assertHit(*setup.ecs, entity_two, "thing1");
}

TEST(ComponentManager, ExecuteWhenOneEntityThroughRightOfAnotherShouldMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 3, 2, 2, 1, "thing1");
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 1, 1, 3, 3, "thing2");
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHit(*setup.ecs, entity_one, "thing2");
  assertHit(*setup.ecs, entity_two, "thing1");
}

TEST(ComponentManager, ExecuteWhenEntitiesBarelyTouchingShouldMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 0, 0, 1, 1, "thing1");
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 1, 1, 3, 3, "thing2");
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHit(*setup.ecs, entity_one, "thing2");
  assertHit(*setup.ecs, entity_two, "thing1");
}

TEST(ComponentManager, ExecuteWhenEntityAboveOtherShouldNOTMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 2, 5, 1, 1, "thing1");
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 1, 1, 3, 3, "thing2");
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHitNothing(*setup.ecs, entity_one);
  assertHitNothing(*setup.ecs, entity_two);
}

TEST(ComponentManager, ExecuteWhenEntityBelowOtherShouldNOTMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 2, -1, 1, 1, "thing1");
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 1, 1, 3, 3, "thing2");
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHitNothing(*setup.ecs, entity_one);
  assertHitNothing(*setup.ecs, entity_two);
}

TEST(ComponentManager, ExecuteWhenEntityRightOfOtherShouldNOTMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 5, 2, 1, 1, "thing1");
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 1, 1, 3, 3, "thing2");
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHitNothing(*setup.ecs, entity_one);
  assertHitNothing(*setup.ecs, entity_two);
}

TEST(ComponentManager, ExecuteWhenEntityLeftOfOtherShouldNOTMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, -1, 2, 1, 1, "thing1");
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 1, 1, 3, 3, "thing2");
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHitNothing(*setup.ecs, entity_one);
  assertHitNothing(*setup.ecs, entity_two);
}

TEST(ComponentManager, ExecuteMultipleEntitiesTouchingOneEntityShouldMarkOnlyOneOfThemAsHavingHitIt) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 1, 1, 3, 3, "thing1");
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 3, 3, 1, 1, "thing2");
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());
  auto entity_three = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1, "thing3");
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());
  auto entity_four = createEntityWithHitbox(*setup.ecs, 1, 3, 1, 1, "thing4");
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHit(*setup.ecs, entity_one, "thing2");
  assertHit(*setup.ecs, entity_two, "thing1");
  assertHitNothing(*setup.ecs, entity_three);
  assertHitNothing(*setup.ecs, entity_four);
}

TEST(ComponentManager, ExecuteMultipleHitsInOneFrameShouldMarkAllOfThem) {
  TestSetup setup = setupTest();
  auto entity_one = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1, "thing1");
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto entity_two = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1, "thing2");
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());
  auto entity_three = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1, "thing3");
  setup.system.add_entity_if_matches(entity_three, setup.ecs->components());
  auto entity_threehalf = createEntityWithHitbox(*setup.ecs, 100, 100, 1, 1, "thing3.5");
  setup.system.add_entity_if_matches(entity_threehalf, setup.ecs->components());
  auto entity_four = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1, "thing4");
  setup.system.add_entity_if_matches(entity_four, setup.ecs->components());
  auto entity_five = createEntityWithHitbox(*setup.ecs, 3, 3, 3, 3, "thing5");
  setup.system.add_entity_if_matches(entity_five, setup.ecs->components());
  auto entity_six = createEntityWithHitbox(*setup.ecs, 4, 4, 1, 1, "thing6");
  setup.system.add_entity_if_matches(entity_six, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHit(*setup.ecs, entity_one, "thing2");
  assertHit(*setup.ecs, entity_two, "thing1");
  assertHit(*setup.ecs, entity_three, "thing4");
  assertHitNothing(*setup.ecs, entity_threehalf);
  assertHit(*setup.ecs, entity_four, "thing3");
  assertHit(*setup.ecs, entity_five, "thing6");
  assertHit(*setup.ecs, entity_six, "thing5");
}

TEST(ComponentManager, ExecuteEntityHitSomethingOnPreviousTickButNotThisTickShouldNOTBeMarkedAsHittingAnything) {
  TestSetup setup = setupTest();

  auto entity_one = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1, "thing1");
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto e1_col = setup.ecs->components().get<components::Collision>(entity_one);
  e1_col.hit_something_this_tick = true;
  e1_col.type_of_what_i_hit = "test123";
  setup.ecs->components().set(entity_one, e1_col);

  setup.system.execute(*setup.ecs);

  assertHitNothing(*setup.ecs, entity_one);
}

TEST(ComponentManager, ExecuteWhenEntityNotActiveShouldNOTBeIncludedInCollisionChecks) {
  TestSetup setup = setupTest();

  auto entity_one = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1, "thing1");
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto e1_col = setup.ecs->components().get<components::Collision>(entity_one);
  e1_col.active = false;
  setup.ecs->components().set(entity_one, e1_col);

  auto entity_two = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1, "thing2");
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHitNothing(*setup.ecs, entity_one);
  assertHitNothing(*setup.ecs, entity_two);
}

TEST(ComponentManager, RemoveEntityWhenEntityRemovedShouldNOTBeIncludedInCollisionChecks) {
  TestSetup setup = setupTest();

  auto entity_one = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1, "thing1");
  setup.system.add_entity_if_matches(entity_one, setup.ecs->components());
  auto e1_col = setup.ecs->components().get<components::Collision>(entity_one);
  setup.system.remove_entity(entity_one);

  auto entity_two = createEntityWithHitbox(*setup.ecs, 1, 1, 1, 1, "thing2");
  setup.system.add_entity_if_matches(entity_two, setup.ecs->components());

  setup.system.execute(*setup.ecs);

  assertHitNothing(*setup.ecs, entity_one);
  assertHitNothing(*setup.ecs, entity_two);
}