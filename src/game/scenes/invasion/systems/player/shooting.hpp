#pragma once

#include "framework/system.hpp"
#include <entt.hpp>
#include <functional>
#include <set>

namespace systems::player {

class Shooting : public framework::System {
  private:
    std::function<entt::entity(entt::registry &, float, float)> add_projectile;
    std::set<entt::entity> active_projectiles;
    std::function<void(entt::registry &, entt::entity)> add_muzzle_flash;

  public:
    Shooting(
        std::function<entt::entity(entt::registry &ecs, float, float)> add_projectile,
        std::function<void(entt::registry &, entt::entity)> add_muzzle_flash
    );
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems::player