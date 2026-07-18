#pragma once

#include "framework/system.hpp"
#include <entt.hpp>
#include <functional>

namespace systems {

class ExplodeOnDefeat : public framework::System {
  private:
    std::function<void(entt::registry &, float, float)> add_explosion;

  public:
    ExplodeOnDefeat(std::function<void(entt::registry &, float, float)> add_explosion);
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems