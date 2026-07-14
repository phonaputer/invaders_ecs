#include "game/systems/position_following.hpp"
#include "framework/system.hpp"
#include "game/components/deletable.hpp"
#include "game/components/position.hpp"
#include "game/components/position_following.hpp"

namespace systems {

void PositionFollowing::execute(framework::ExecuteCtx &ctx) {
  auto view = ctx.ecs.view<components::PositionFollowing, components::Position>();

  for (auto [entity, following, position] : view.each()) {
    if (!ctx.ecs.valid(following.leader)) {
      ctx.ecs.emplace<components::Deleteable>(entity);
      continue;
    }

    auto leader_position = ctx.ecs.get<components::Position>(following.leader);

    position.x = leader_position.x;
    position.y = leader_position.y;
  }
}

} // namespace systems