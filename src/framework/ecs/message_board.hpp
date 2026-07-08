#pragma once

#include <any>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace ecs {

class MessageBoard {
  private:
    std::unordered_map<std::type_index, std::vector<std::any>> messages;
    std::unordered_map<std::type_index, std::any> singleton_messages;

  public:
    template <typename T> void push_back(T message);
    template <typename T> std::vector<T> get_all() const;
    void clear_all();

    template <typename T> void set_singleton(T message);
    template <typename T> T get_singleton() const;
};

} // namespace ecs

#include "framework/ecs/message_board.tpp"