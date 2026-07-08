#include "framework/ecs/message_board.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <ostream>

struct TestMessage {
    int id;
    bool operator==(const TestMessage &) const = default;
    friend void PrintTo(const TestMessage &tm, std::ostream *os) {
      *os << "TestMessage(" << tm.id << ")";
    }
};

struct TestMessageTwo {
    int id_two;
    bool operator==(const TestMessageTwo &) const = default;
    friend void PrintTo(const TestMessageTwo &tm, std::ostream *os) {
      *os << "TestMessageTwo(" << tm.id_two << ")";
    }
};

TEST(MessageBoard, GetAllWhenNoMessagesExistShouldReturnEmptyVec) {
  auto mb = std::make_unique<ecs::MessageBoard>();

  auto result = mb->get_all<TestMessage>();

  EXPECT_TRUE(result.empty());
}

TEST(MessageBoard, GetAllWhenSeveralMessagesExistShouldReturnAllInOrder) {
  auto mb = std::make_unique<ecs::MessageBoard>();
  auto m1 = TestMessage{.id = 1};
  auto m2 = TestMessage{.id = 2};
  auto m3 = TestMessage{.id = 3};
  mb->push_back(m1);
  mb->push_back(m2);
  mb->push_back(m3);

  auto result = mb->get_all<TestMessage>();

  std::vector<TestMessage> expected = {m1, m2, m3};
  EXPECT_THAT(expected, testing::ElementsAreArray(result));
}

TEST(MessageBoard, SetWithMessagesOfMultipleTypesShouldStoreDifferentTypesSeparately) {
  auto mb = std::make_unique<ecs::MessageBoard>();
  auto m1 = TestMessage{.id = 1};
  mb->push_back(m1);
  auto m2 = TestMessageTwo{.id_two = 2};
  mb->push_back(m2);

  auto result = mb->get_all<TestMessage>();

  std::vector<TestMessage> expected_tm = {m1};
  EXPECT_THAT(expected_tm, testing::ElementsAreArray(mb->get_all<TestMessage>()));
  std::vector<TestMessageTwo> expected_tm2 = {m2};
  EXPECT_THAT(expected_tm2, testing::ElementsAreArray(mb->get_all<TestMessageTwo>()));
}

TEST(MessageBoard, ClearShouldRemoveAllMessagesOfAllTypes) {
  auto mb = std::make_unique<ecs::MessageBoard>();
  auto m1 = TestMessage{.id = 1};
  mb->push_back(m1);
  auto m2 = TestMessageTwo{.id_two = 2};
  mb->push_back(m2);

  mb->clear_all();

  auto result = mb->get_all<TestMessage>();
  EXPECT_TRUE(result.empty());
}

TEST(MessageBoard, GetSingletonAfterSettingShouldReturnTheSetValue) {
  auto mb = std::make_unique<ecs::MessageBoard>();
  auto message = TestMessage{
      .id = 111,
  };
  mb->set_singleton(message);

  auto result = mb->get_singleton<TestMessage>();

  EXPECT_EQ(message, result);
}

TEST(MessageBoard, DoubleSetSingletonShouldLeaveOnlyTheLastValuePresent) {
  auto mb = std::make_unique<ecs::MessageBoard>();
  auto message = TestMessage{
      .id = 111,
  };
  mb->set_singleton(message);
  auto messageTwo = TestMessage{
      .id = 222,
  };
  mb->set_singleton(messageTwo);

  auto result = mb->get_singleton<TestMessage>();

  EXPECT_EQ(messageTwo, result);
}

TEST(MessageBoard, SetDifferentTypeSingletonsShouldNotInterfereWithEachOther) {
  auto mb = std::make_unique<ecs::MessageBoard>();
  auto message = TestMessage{
      .id = 111,
  };
  mb->set_singleton(message);
  auto messageTwo = TestMessageTwo{
      .id_two = 222,
  };
  mb->set_singleton(messageTwo);

  EXPECT_EQ(message, mb->get_singleton<TestMessage>());
  EXPECT_EQ(messageTwo, mb->get_singleton<TestMessageTwo>());
}