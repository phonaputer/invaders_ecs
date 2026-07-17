#pragma once

#include "framework/scene.hpp"

namespace start_screen {

class Scene : public framework::Scene {
    void initialize(framework::SceneInitializationContext ctx) override;
};

} // namespace start_screen