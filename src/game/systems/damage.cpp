#include "game/systems/damage.hpp"
#include "game/components/collided_with.hpp"
#include "game/components/damage_dealer.hpp"
#include "game/components/damage_type_enum.hpp"
#include "game/components/deletable.hpp"
#include "game/components/hitpoints.hpp"
#include "game/events/collision_occurred.hpp"
#include "game/events/defeated.hpp"
#include "game/events/player_scored.hpp"
#include "game/events/took_damage.hpp"
#include <flecs.h>
#include <set>

namespace systems {

void damage(flecs::world world) {
  world.system<events::CollisionOccurred>("Damage").each(
      [](flecs::iter &it, size_t index, flecs::entity ent, events::CollisionOccurred &collision) {
        ent.destruct();

        if (!collision.who_am_i.has<components::DamageDealer>() && !collision.who_i_hit.has<components::Hitpoints>()) {
          return;
        }

        auto damage = collision.who_am_i.get<components::DamageDealer>();
        auto hitpoints = collision.who_i_hit.get<components::Hitpoints>();

        if (!(damage.type & hitpoints.susceptible_to).any()) {
          return;
        }

        hitpoints.cur_hitpoints -= damage.amount;
        collision.who_i_hit.set(hitpoints);

        collision.who_i_hit.set<events::TookDamage>({.amount = damage.amount});

        if (hitpoints.cur_hitpoints > 0) {
          return;
        }

        collision.who_i_hit.add<events::Defeated>();
        collision.who_i_hit.add<components::Deleteable>();

        if (hitpoints.grants_score) {
          it.world().entity().set<events::PlayerScored>({.score = hitpoints.score_value});
        }
      }
  );
}

} // namespace systems