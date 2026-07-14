#include "game/systems/explode_on_defeat.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "game/components/explode_on_defeat.hpp"
#include "game/components/position.hpp"
#include "game/events/defeated.hpp"
#include <set>

namespace systems {

void explode_on_defeat(flecs::world world, std::function<void(flecs::world, core::Point)> add_explosion) {
  world.system<const components::ExplodeOnDefeat, const events::Defeated, const components::Position>("ExplodeOnDefeat")
      .each([add_explosion](
                flecs::iter &it,
                const components::ExplodeOnDefeat e,
                const events::Defeated d,
                const components::Position p
            ) { add_explosion(it.world(), {p.x, p.y}); });
}

} // namespace systems