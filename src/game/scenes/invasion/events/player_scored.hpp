#pragma once

namespace events {

struct PlayerScored {
    unsigned int score;
    bool operator==(const PlayerScored &) const = default;
};

} // namespace events