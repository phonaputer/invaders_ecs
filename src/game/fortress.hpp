#pragma once

#include "framework/ecs/ecs.hpp"

namespace game {

void add_fortresses(ecs::ECS &ecs);

void add_fortress(ecs::ECS &ecs, float x, float y);

struct AddFortressTileArgs {
    float x;
    float y;
    float img_src_x;
    float img_src_y;
};

void add_fortress_tile(ecs::ECS &ecs, AddFortressTileArgs args);

}