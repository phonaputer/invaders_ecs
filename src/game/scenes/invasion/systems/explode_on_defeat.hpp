#pragma once

#include "core/point.hpp"
#include "framework/system.hpp"
#include <entt.hpp>
#include <functional>

namespace systems {

class ExplodeOnDefeat : public framework::System {
  private:
    std::function<void(entt::registry &, core::Point)> add_explosion;

  public:
    ExplodeOnDefeat(std::function<void(entt::registry &, core::Point)> add_explosion);
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems