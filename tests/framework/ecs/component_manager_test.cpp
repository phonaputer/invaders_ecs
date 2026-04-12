#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/entity.hpp"
#include <gtest/gtest.h>

struct TestComponent {
    int id;
    bool operator==(const TestComponent &) const = default;
};

struct TestComponentTwo {
    int id_two;
    bool operator==(const TestComponentTwo &) const = default;
};

TEST(ComponentManager, GetWhenEntityComponentExistShouldReturnTheComponent) {
  auto cm = std::make_unique<ecs::ComponentManager>();
  ecs::Entity entity = 123;
  auto component = TestComponent{
      .id = 456,
  };
  cm->set(entity, component);

  auto result = cm->get<TestComponent>(entity);

  EXPECT_EQ(component, result);
}

TEST(ComponentManager, GetWhenEntityComponentDoesNotExistShouldThrowOutOfRangeError) {
  auto cm = std::make_unique<ecs::ComponentManager>();
  ecs::Entity entity = 123;

  try {
    cm->get<TestComponent>(entity);
  } catch (const std::out_of_range &err) {
    EXPECT_STREQ("unordered_map::at", err.what());
    return;
  }

  FAIL() << "Expected an error";
}

TEST(ComponentManager, HasWhenNonSetEntityComponentShouldReturnFalse) {
  auto cm = std::make_unique<ecs::ComponentManager>();
  ecs::Entity entity = 123;

  EXPECT_FALSE(cm->has<TestComponent>(entity));
}

TEST(ComponentManager, HasWhenEntityHasAComponentButNotTheRequestedOneShouldReturnFalse) {
  auto cm = std::make_unique<ecs::ComponentManager>();
  ecs::Entity entity = 123;
  auto component = TestComponent{
      .id = 456,
  };
  cm->set(entity, component);

  EXPECT_FALSE(cm->has<TestComponentTwo>(entity));
}

TEST(ComponentManager, SetWhenEntityComponentDoesNotExistShouldInsertEntityComponent) {
  auto cm = std::make_unique<ecs::ComponentManager>();
  ecs::Entity entity = 123;
  auto component = TestComponent{
      .id = 456,
  };

  cm->set(entity, component);

  auto result = cm->get<TestComponent>(entity);
  EXPECT_EQ(component, result);
}

TEST(ComponentManager, SetWhenEntityComponentAlreadyExistsShouldOverwriteTheExistingEntityComponent) {
  auto cm = std::make_unique<ecs::ComponentManager>();
  ecs::Entity entity = 123;

  auto component = TestComponent{
      .id = 456,
  };
  cm->set(entity, component);
  EXPECT_EQ(component, cm->get<TestComponent>(entity));

  auto componentTwo = TestComponent{
      .id = 789,
  };
  cm->set(entity, componentTwo);
  EXPECT_EQ(componentTwo, cm->get<TestComponent>(entity));
}

TEST(ComponentManager, DeleteComponentWhenNoEntityHasThisComponentShouldDoNothing) {
  auto cm = std::make_unique<ecs::ComponentManager>();
  ecs::Entity entity = 123;
  cm->delete_component<TestComponent>(entity);

  EXPECT_FALSE(cm->has<TestComponent>(entity));
}

TEST(ComponentManager, DeleteComponentWhenAnEntityHasThisComponentButTheRequestedEntityDoesNotShouldDoNothing) {
  auto cm = std::make_unique<ecs::ComponentManager>();
  ecs::Entity entityOne = 1;
  auto component = TestComponent{
      .id = 456,
  };
  cm->set(entityOne, component);
  ecs::Entity entityTwo = 2;

  cm->delete_component<TestComponent>(entityTwo);

  EXPECT_FALSE(cm->has<TestComponent>(entityTwo));
  EXPECT_TRUE(cm->has<TestComponent>(entityOne));
}

TEST(ComponentManager, DeleteComponentWhenEntityHasAComponentButNotTheRequestedComponentDoesNotShouldDoNothing) {
  auto cm = std::make_unique<ecs::ComponentManager>();
  ecs::Entity entity = 1;
  auto component = TestComponent{
      .id = 456,
  };
  cm->set(entity, component);

  cm->delete_component<TestComponentTwo>(entity);

  EXPECT_TRUE(cm->has<TestComponent>(entity));
}

TEST(ComponentManager, DeleteComponentWhenEntityHasThisComponentShouldDeleteTheComponent) {
  auto cm = std::make_unique<ecs::ComponentManager>();
  ecs::Entity entity = 1;
  auto component = TestComponent{
      .id = 111,
  };
  auto componentTwo = TestComponentTwo{
      .id_two = 222,
  };
  cm->set(entity, component);
  cm->set(entity, componentTwo);

  cm->delete_component<TestComponentTwo>(entity);

  EXPECT_FALSE(cm->has<TestComponentTwo>(entity));
  EXPECT_TRUE(cm->has<TestComponent>(entity));
}

TEST(ComponentManager, DeleteEntityWhenEntityDoesNotExistShouldDoNothing) {
  auto cm = std::make_unique<ecs::ComponentManager>();
  ecs::Entity entityOne = 1;
  auto component = TestComponent{
      .id = 111,
  };
  auto componentTwo = TestComponentTwo{
      .id_two = 222,
  };
  cm->set(entityOne, component);
  cm->set(entityOne, componentTwo);
  ecs::Entity entityTwo = 2;

  cm->delete_entity(entityTwo);

  EXPECT_EQ(componentTwo, cm->get<TestComponentTwo>(entityOne));
  EXPECT_EQ(component, cm->get<TestComponent>(entityOne));
}

TEST(ComponentManager, DeleteEntityWhenEntityDoesExistShouldDeleteAllComponentsForThisEntity) {
  auto cm = std::make_unique<ecs::ComponentManager>();
  ecs::Entity entity = 1;
  auto component = TestComponent{
      .id = 111,
  };
  auto componentTwo = TestComponentTwo{
      .id_two = 222,
  };
  cm->set(entity, component);
  cm->set(entity, componentTwo);

  cm->delete_entity(entity);

  EXPECT_FALSE(cm->has<TestComponentTwo>(entity));
  EXPECT_FALSE(cm->has<TestComponent>(entity));
}