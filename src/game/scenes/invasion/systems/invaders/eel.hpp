#pragma once

#include "framework/system.hpp"
#include <functional>
#include <random>

namespace systems::invaders {

class Eel : public framework::System {
  private:
    std::function<void(entt::registry &, std::mt19937 &)> spawn_eel;
    std::mt19937 rand_gen;

  public:
    Eel(std::function<void(entt::registry &, std::mt19937 &)> spawn_eel, unsigned int rand_seed);
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems::invaders