#include "gallia/systems/score.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "gallia/components/singleton/hud_stats.hpp"
#include "gallia/events/player_scored.hpp"
#include <format>

namespace systems {

void Score::remove_entity([[maybe_unused]] ecs::Entity entity) {
}

void Score::add_entity_if_matches(
    [[maybe_unused]] ecs::Entity entity, [[maybe_unused]] ecs::ComponentManager &entity_components
) {
}

void Score::execute(ecs::ECS &ecs) {
  unsigned int total_score_increment = 0;
  for (const auto &event : ecs.events().get_all<events::PlayerScored>()) {
    total_score_increment += event.score;
  }

  auto stats = ecs.components().get_singleton<components::singleton::HUDStats>();

  stats.score += total_score_increment;
  if (stats.score > stats.high_score) {
    stats.high_score = stats.score;
  }

  ecs.components().set_singleton(stats);
}

} // namespace systems