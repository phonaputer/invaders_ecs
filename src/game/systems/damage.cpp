#include "game/systems/damage.hpp"
#include "framework/system.hpp"
#include "game/components/damage_dealer.hpp"
#include "game/components/damage_type_enum.hpp"
#include "game/components/deletable.hpp"
#include "game/components/hitpoints.hpp"
#include "game/events/collision_occurred.hpp"
#include "game/events/defeated.hpp"
#include "game/events/player_scored.hpp"
#include "game/events/took_damage.hpp"
#include <entt.hpp>

namespace systems {

void Damage::execute(framework::ExecuteCtx &ctx) {
  auto collisions = ctx.events.get_all<events::CollisionOccurred>();

  for (const auto &collision : collisions) {
    if (!ctx.ecs.all_of<components::DamageDealer>(collision.who_am_i)) {
      continue;
    }

    if (!ctx.ecs.all_of<components::Hitpoints>(collision.who_i_hit)) {
      continue;
    }

    auto damage = ctx.ecs.get<components::DamageDealer>(collision.who_am_i);
    auto hitpoints = ctx.ecs.get<components::Hitpoints>(collision.who_i_hit);

    if (!(damage.type & hitpoints.susceptible_to).any()) {
      continue;
    }

    hitpoints.cur_hitpoints -= damage.amount;
    ctx.ecs.replace<components::Hitpoints>(collision.who_i_hit, hitpoints);

    ctx.events.push_back(events::TookDamage{.entity = collision.who_i_hit, .amount = damage.amount});

    if (hitpoints.cur_hitpoints > 0) {
      continue;
    }

    ctx.events.push_back(events::Defeated{.entity = collision.who_i_hit});

    ctx.ecs.emplace<components::Deleteable>(collision.who_i_hit);

    if (hitpoints.grants_score) {
      ctx.events.push_back(
          events::PlayerScored{
              .score = hitpoints.score_value,
          }
      );
    }
  }
}

} // namespace systems