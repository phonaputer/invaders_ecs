#pragma once

#include "framework/game/player_input.hpp"
#include <set>

namespace game {

class PlayerInputManager {
  private:
    std::set<PlayerInput> current_inputs;
    std::set<PlayerInput> current_initiations;
    std::set<PlayerInput> previous_tick_inputs;

  public:
    bool is_engaged(PlayerInput input) const;
    bool is_initiated(PlayerInput input) const;
    void engage(PlayerInput input);
    void disengage(PlayerInput input);
    void update();
};

}