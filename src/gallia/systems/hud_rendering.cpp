#include "gallia/systems/hud_rendering.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "framework/game/constants.hpp"
#include "framework/game/renderer.hpp"
#include "gallia/components/singleton/hud_stats.hpp"
#include "gallia/util/text_renderer.hpp"
#include <format>

namespace systems {

HUDRendering::HUDRendering(game::Renderer &renderer)
    : text_renderer{renderer} {
}

void HUDRendering::remove_entity([[maybe_unused]] ecs::Entity entity) {
}

void HUDRendering::add_entity_if_matches(
    [[maybe_unused]] ecs::Entity entity, [[maybe_unused]] ecs::ComponentManager &entity_components
) {
}

// This system hardcodes a lot
// Probably worth it though to avoid adding complexity elsewhere
void HUDRendering::execute(ecs::ECS &ecs) {
  auto hud_stats = ecs.components().get_singleton<components::singleton::HUDStats>();

  text_renderer.render_text(6, 6, std::format("score:{}", hud_stats.score));
  text_renderer.render_text(game::WINDOW_WIDTH / 2 - 26, 6, std::format("hi-score:{}", hud_stats.high_score));
  text_renderer.render_text(game::WINDOW_WIDTH - 48, 6, std::format("lives:{}", hud_stats.lives));
}

} // namespace systems