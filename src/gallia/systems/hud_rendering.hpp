#pragma once

#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "framework/game/renderer.hpp"
#include "gallia/util/text_renderer.hpp"
#include <set>

namespace systems {

class HUDRendering : public ecs::System {
  private:
    util::TextRenderer text_renderer;

  public:
    HUDRendering(game::Renderer &renderer);
    void remove_entity(ecs::Entity entity) override;
    void add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) override;
    void execute(ecs::ECS &ecs) override;
};

} // namespace systems