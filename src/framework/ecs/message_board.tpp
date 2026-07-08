#pragma once

#include "framework/ecs/message_board.hpp"
#include <any>
#include <cassert>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace ecs {

template <typename T> void MessageBoard::push_back(T message) {
  messages[std::type_index(typeid(T))].push_back(message);
}

template <typename T> std::vector<T> MessageBoard::get_all() const {
  if (!messages.contains(std::type_index(typeid(T)))) {
    return {};
  }

  auto &message_vec = messages.at(std::type_index(typeid(T)));

  std::vector<T> results;
  results.reserve(message_vec.size());

  for (const auto &any_value : message_vec) {
    results.push_back(std::any_cast<T>(any_value));
  }

  return std::move(results);
}

inline void MessageBoard::clear_all() {
  messages.clear();
}

template <typename T> void MessageBoard::set_singleton(T message) {
  singleton_messages[std::type_index(typeid(T))] = message;
}

template <typename T> T MessageBoard::get_singleton() const {
  assert(singleton_messages.contains(std::type_index(typeid(T))));

  auto any_value = singleton_messages.at(std::type_index(typeid(T)));

  return std::any_cast<T>(any_value);
}

} // namespace ecs