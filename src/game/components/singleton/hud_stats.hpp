#pragma once

namespace components::singleton {

struct HUDStats {
    int lives;
    int score;
    int high_score;
    bool game_over;
};

} // namespace components::singleton