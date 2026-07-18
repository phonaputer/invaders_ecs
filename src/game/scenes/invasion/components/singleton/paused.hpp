#pragma once

namespace components::singleton {

struct Paused {
    bool paused;
    unsigned int pause_counter = 0;
};

} // namespace components::singleton