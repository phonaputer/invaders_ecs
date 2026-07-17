#include "game/fortress.hpp"
#include "game/assets/asset_enums.hpp"
#include "game/components/collision.hpp"
#include "game/components/damage_dealer.hpp"
#include "game/components/damage_type_enum.hpp"
#include "game/components/deletable.hpp"
#include "game/components/delete_on_gameover.hpp"
#include "game/components/hitpoints.hpp"
#include "game/components/position.hpp"
#include "game/components/sprite.hpp"
#include "game/components/sprite_offset_on_damage.hpp"
#include <entt.hpp>

namespace game {

constexpr float TILE_DRAW_WIDTH = 4;
constexpr float TILE_DRAW_HEIGHT = 4;
constexpr float TILE_SRC_WIDTH = 4;
constexpr float TILE_SRC_HEIGHT = 4;

constexpr float TOP_LEFT_SRC_X = 48;
constexpr float TOP_LEFT_SRC_Y = 48;

struct AddFortressTileArgs {
    float x;
    float y;
    float img_src_x;
    float img_src_y;
};

void add_fortress_tile(entt::registry &ecs, AddFortressTileArgs args) {
  const auto entity = ecs.create();

  ecs.emplace<components::DeleteOnGameOver>(entity);

  components::DamageTypeSet susceptible_damage_types;
  susceptible_damage_types.set(components::damage_type_to_index(components::DamageType::Player_Projectile));
  susceptible_damage_types.set(components::damage_type_to_index(components::DamageType::Alien_Projectile));
  ecs.emplace<components::Hitpoints>(
      entity,
      components::Hitpoints{
          .susceptible_to = susceptible_damage_types,
          .cur_hitpoints = 3,
      }
  );

  components::DamageTypeSet deal_damage_types;
  deal_damage_types.set(components::damage_type_to_index(components::DamageType::Fortress));
  ecs.emplace<components::DamageDealer>(
      entity,
      components::DamageDealer{
          .type = deal_damage_types,
          .amount = 1,
      }
  );

  ecs.emplace<components::Position>(
      entity,
      components::Position{
          .x = args.x,
          .y = args.y,
          .w = TILE_DRAW_WIDTH,
          .h = TILE_DRAW_HEIGHT,
          .z = 20,
      }
  );

  ecs.emplace<components::Collision>(
      entity,
      components::Collision{
          .hitbox_offset_x = 0,
          .hitbox_offset_y = 0,
          .hitbox_w = TILE_DRAW_WIDTH,
          .hitbox_h = TILE_DRAW_HEIGHT,
      }
  );

  ecs.emplace<components::Sprite>(
      entity,
      components::Sprite{
          .image = assets::Image::InvadersSpritesheet,
          .src_x = args.img_src_x,
          .src_y = args.img_src_y,
          .src_width = TILE_SRC_WIDTH,
          .src_height = TILE_SRC_HEIGHT,
          .dst_width = TILE_DRAW_WIDTH,
          .dst_height = TILE_DRAW_HEIGHT,
      }
  );

  ecs.emplace<components::SpriteOffsetOnDamage>(
      entity,
      components::SpriteOffsetOnDamage{
          .offset_x = 6 * TILE_SRC_WIDTH,
          .offset_y = 0,
      }
  );
}

void add_fortress(entt::registry &ecs, float start_x, float start_y) {
  for (int col = 0; col < 6; col++) {
    float x = start_x + col * TILE_DRAW_WIDTH;
    float src_x = TOP_LEFT_SRC_X + col * TILE_SRC_WIDTH;

    for (int row = 0; row < 4; row++) {
      if (row == 3 && (col == 2 || col == 3)) {
        continue;
      }

      float y = start_y + row * TILE_DRAW_HEIGHT;
      float src_y = TOP_LEFT_SRC_Y + row * TILE_SRC_HEIGHT;

      add_fortress_tile(
          ecs,
          AddFortressTileArgs{
              .x = x,
              .y = y,
              .img_src_x = src_x,
              .img_src_y = src_y,
          }
      );
    }
  }
}

void add_fortresses(entt::registry &ecs) {
  add_fortress(ecs, 29, 235);
  add_fortress(ecs, 72, 235);
  add_fortress(ecs, 119, 235);
  add_fortress(ecs, 166, 235);
}

} // namespace game