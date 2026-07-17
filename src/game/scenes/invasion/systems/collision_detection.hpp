#pragma once

#include "framework/system.hpp"
#include <entt.hpp>
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

struct BucketKey {
    int x;
    int y;

    auto operator<=>(const BucketKey &) const = default;
};

class CollisionDetection : public framework::System {
  private:
    static constexpr int BUCKET_WIDTH = 15;
    static constexpr int BUCKET_HEIGHT = 15;

    std::set<BucketKey> buckets_to_check;
    std::unordered_map<int, std::unordered_map<int, std::vector<Hitbox>>> hitbox_buckets;

    BucketKey to_bucket_key(float x, float y);
    void fill_buckets(entt::registry &ecs);
    void add_to_bucket(Hitbox hitbox, int bucket_x, int bucket_y);
    void clear_buckets();
    void check_collisions(framework::ExecuteCtx &ctx, const std::vector<Hitbox> hitboxes);
    void emit_collided_if_didnt_already(framework::ExecuteCtx &ctx, entt::entity right, entt::entity left);
    bool are_touching(Hitbox right, Hitbox left);

  public:
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems