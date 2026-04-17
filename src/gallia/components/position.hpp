#pragma once

namespace components {

struct Position {
    float x;
    float y;
    // TODO these kind of don't belong here maybe since they mean different things for collision and sprite drawing?
    float w;
    float h;
    unsigned int z;
};

} // namespace components