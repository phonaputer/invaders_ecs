#pragma once

namespace components {

struct Lifetime {
    unsigned int ticks;
    unsigned int tick_counter = 0;
};

} // namespace components