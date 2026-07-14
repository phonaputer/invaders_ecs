#include "game/systems/deletion.hpp"
#include "framework/system.hpp"
#include "game/components/deletable.hpp"

namespace systems {

void Deletion::execute(framework::ExecuteCtx &ctx) {
  auto view = ctx.ecs.view<components::Deleteable>();

  ctx.ecs.destroy(view.begin(), view.end());
}

} // namespace systems