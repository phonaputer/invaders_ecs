#pragma once

#include "core/point.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include <functional>
#include <optional>
#include <set>

namespace systems::player {

class Defeat : public framework::system {
  private:
    static constexpr unsigned int DEFEAT_PAUSE_TICKS = 100;

    std::optional<ecs::Entity> player = std::nullopt;
    std::set<ecs::Entity> deletable_entities;

    unsigned int defeat_pause_counter = 0;
    bool defeat_pause_ongoing = false;
    bool game_is_over = false;
    std::function<void(ecs::ECS &, core::Point, unsigned int)> add_explosion;
    std::function<void(ecs::ECS &)> add_player;
    std::function<void(ecs::ECS &)> add_fortresses;

    void handle_defeat_if_any(ecs::ECS &ecs);
    void handle_defeat(ecs::ECS &ecs, ecs::Entity player_entity);
    void handle_ongoing_pause(ecs::ECS &ecs);
    void delete_all_entities(ecs::ECS &ecs);

  public:
    Defeat(
        std::function<void(ecs::ECS &, core::Point, unsigned int)> add_explosion,
        std::function<void(ecs::ECS &)> add_player,
        std::function<void(ecs::ECS &)> add_fortresses
    );
    void remove_entity(ecs::Entity entity) override;
    void add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) override;
    void execute(ecs::ECS &ecs) override;
};

} // namespace systems::player