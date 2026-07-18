#pragma once

namespace components::singleton {

struct InvaderOrchestration {
    bool move_right;
    int move_counter;
    int shoot_counter;
    unsigned int arp_idx;
};

} // namespace components::singleton