#pragma once

#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include <cstdint>
#include <set>
#include <unordered_map>
#include <vector>

namespace systems {

struct Hitbox {
    ecs::Entity entity;
    float x;
    float y;
    float w;
    float h;
};

class CollisionDetection : public framework::system {
  private:
    // These must always be larger than the max hitbox in game to avoid a
    // really big hitbox encompassing an entire bucket without any of its
    // corners falling in it. Currently the largest is the player with a width
    // of 15.
    static constexpr std::uint16_t BUCKET_WIDTH = 17;
    static constexpr std::uint16_t BUCKET_HEIGHT = 17;

    std::set<ecs::Entity> entities;
    std::unordered_map<std::int32_t, std::vector<Hitbox>> hitbox_buckets;

    std::int32_t to_bucket_key(float x, float y);
    void fill_buckets(ecs::ECS &ecs);
    void add_to_bucket(Hitbox hitbox, float x, float y);
    void clear_buckets();
    void check_collisions(ecs::ECS &ecs, const std::vector<Hitbox> hitboxes);
    void emit_collided_if_didnt_already(ecs::ECS &ecs, ecs::Entity right, ecs::Entity left);
    bool are_touching(Hitbox right, Hitbox left);

  public:
    void remove_entity(ecs::Entity entity) override;
    void add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) override;
    void execute(ecs::ECS &ecs) override;
};

} // namespace systems