include(FetchContent)

FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-3.4.2
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(SDL3)

add_library(vendor_lib INTERFACE)
target_link_libraries(vendor_lib INTERFACE SDL3::SDL3)
target_include_directories(vendor_lib INTERFACE entt-3.16.0)