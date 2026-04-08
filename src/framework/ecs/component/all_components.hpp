#pragma once

#include "component_manager.hpp"
#include "component_type.hpp"
#include <unordered_map>

namespace component {

std::unordered_map<component::Type, Manager> get_all();

} // namespace component
