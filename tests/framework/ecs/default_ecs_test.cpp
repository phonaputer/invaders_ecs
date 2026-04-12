#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/default_ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class StubSystem : public ecs::System {
  public:
    MOCK_METHOD(void, remove_entity, (ecs::Entity entity), (override));
    MOCK_METHOD(void, add_entity_if_matches, (ecs::Entity entity, ecs::ComponentManager &components), (override));
    MOCK_METHOD(void, execute, (ecs::ComponentManager & components), (override));
};

TEST(DefaultECS, NewEntityWhenGettingSeveralEntitiesShouldReturnANewIDForEach) {
  auto ecs = std::make_unique<ecs::DefaultECS>(std::make_unique<ecs::ComponentManager>());

  auto entityOne = ecs->new_entity();
  auto entityTwo = ecs->new_entity();
  auto entityThree = ecs->new_entity();

  EXPECT_TRUE(entityOne != entityTwo);
  EXPECT_TRUE(entityOne != entityThree);
  EXPECT_TRUE(entityTwo != entityThree);
}

TEST(DefaultECS, UpdateWhenUpdateSystemsArePresentShouldInvokeAllOfThem) {
  auto ecs = std::make_unique<ecs::DefaultECS>(std::make_unique<ecs::ComponentManager>());

  auto systemOne = std::make_unique<StubSystem>();
  EXPECT_CALL(*systemOne, execute(testing::_)).Times(1);
  ecs->add_update_system(std::move(systemOne));

  auto systemTwo = std::make_unique<StubSystem>();
  EXPECT_CALL(*systemTwo, execute(testing::_)).Times(1);
  ecs->add_update_system(std::move(systemTwo));

  ecs->update();
}

TEST(DefaultECS, UpdateWhenUpdateAndDrawSystemsArePresentShouldInvokeOnlyUpdateSystems) {
  auto ecs = std::make_unique<ecs::DefaultECS>(std::make_unique<ecs::ComponentManager>());

  auto systemOne = std::make_unique<StubSystem>();
  EXPECT_CALL(*systemOne, execute(testing::_)).Times(1);
  ecs->add_update_system(std::move(systemOne));

  auto systemTwo = std::make_unique<StubSystem>();
  EXPECT_CALL(*systemTwo, execute(testing::_)).Times(0);
  ecs->add_draw_system(std::move(systemTwo));

  ecs->update();
}

TEST(DefaultECS, DrawWhenDrawSystemsArePresentShouldInvokeAllOfThem) {
  auto ecs = std::make_unique<ecs::DefaultECS>(std::make_unique<ecs::ComponentManager>());

  auto systemOne = std::make_unique<StubSystem>();
  EXPECT_CALL(*systemOne, execute(testing::_)).Times(1);
  ecs->add_draw_system(std::move(systemOne));

  auto systemTwo = std::make_unique<StubSystem>();
  EXPECT_CALL(*systemTwo, execute(testing::_)).Times(1);
  ecs->add_draw_system(std::move(systemTwo));

  ecs->draw();
}

TEST(DefaultECS, DrawWhenDrawAndUpdateSystemsArePresentShouldInvokeOnlyDrawSystems) {
  auto ecs = std::make_unique<ecs::DefaultECS>(std::make_unique<ecs::ComponentManager>());

  auto systemOne = std::make_unique<StubSystem>();
  EXPECT_CALL(*systemOne, execute(testing::_)).Times(0);
  ecs->add_update_system(std::move(systemOne));

  auto systemTwo = std::make_unique<StubSystem>();
  EXPECT_CALL(*systemTwo, execute(testing::_)).Times(1);
  ecs->add_draw_system(std::move(systemTwo));

  ecs->draw();
}

TEST(DefaultECS, DeleteFromSystemsWhenSeveralSystemsArePresentShouldDeleteEntityFromAllOfThem) {
  auto ecs = std::make_unique<ecs::DefaultECS>(std::make_unique<ecs::ComponentManager>());

  ecs::Entity entity = 1;

  auto systemOne = std::make_unique<StubSystem>();
  EXPECT_CALL(*systemOne, remove_entity(entity)).Times(1);
  ecs->add_update_system(std::move(systemOne));

  auto systemTwo = std::make_unique<StubSystem>();
  EXPECT_CALL(*systemTwo, remove_entity(entity)).Times(1);
  ecs->add_draw_system(std::move(systemTwo));

  ecs->delete_from_systems(entity);
}

TEST(DefaultECS, RegisterToSystemsWhenSeveralSystemsArePresentShouldDeleteThenAddEntityToAllSystems) {
  auto ecs = std::make_unique<ecs::DefaultECS>(std::make_unique<ecs::ComponentManager>());

  ecs::Entity entity = 1;

  auto systemOne = std::make_unique<StubSystem>();
  EXPECT_CALL(*systemOne, remove_entity(entity)).Times(1);
  EXPECT_CALL(*systemOne, add_entity_if_matches(entity, testing::_)).Times(1);
  ecs->add_update_system(std::move(systemOne));

  auto systemTwo = std::make_unique<StubSystem>();
  EXPECT_CALL(*systemTwo, remove_entity(entity)).Times(1);
  EXPECT_CALL(*systemTwo, add_entity_if_matches(entity, testing::_)).Times(1);
  ecs->add_draw_system(std::move(systemTwo));

  ecs->register_to_systems(entity);
}
