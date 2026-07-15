#pragma once

#include "framework/system.hpp"
#include <cstdint>
#include <entt.hpp>
#include <set>
#include <unordered_map>
#include <vector>

namespace systems {

struct Hitbox {
    entt::entity entity;
    float x;
    float y;
    float w;
    float h;
};

class CollisionDetection : public framework::System {
  private:
    // These must always be larger than the max hitbox in game to avoid a
    // really big hitbox encompassing an entire bucket without any of its
    // corners falling in it. Currently the largest is the player with a width
    // of 15.
    static constexpr std::uint16_t BUCKET_WIDTH = 17;
    static constexpr std::uint16_t BUCKET_HEIGHT = 17;

    std::set<entt::entity> entities;
    std::unordered_map<std::int32_t, std::vector<Hitbox>> hitbox_buckets;

    std::int32_t to_bucket_key(float x, float y);
    void fill_buckets(entt::registry &ecs);
    void add_to_bucket(Hitbox hitbox, float x, float y);
    void clear_buckets();
    void check_collisions(framework::ExecuteCtx &ctx, const std::vector<Hitbox> hitboxes);
    void emit_collided_if_didnt_already(framework::ExecuteCtx &ctx, entt::entity right, entt::entity left);
    bool are_touching(Hitbox right, Hitbox left);

  public:
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems