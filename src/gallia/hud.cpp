#include "gallia/hud.hpp"
#include "framework/ecs/ecs.hpp"
#include "gallia/components/singleton/hud_stats.hpp"

namespace gallia {

void add_hud_entity(ecs::ECS &ecs) {
  ecs.components().set_singleton(
      components::singleton::HUDStats{
          .lives = 2,
          .score = 0,
          .high_score = 0,
      }
  );
}

} // namespace gallia