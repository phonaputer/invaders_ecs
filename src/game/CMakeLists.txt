target_sources(
  ${PROJECT_NAME}_lib
  PRIVATE
  fortress.cpp
  hud.cpp
  invasion_scene.cpp
  invaders.cpp
  player.cpp
)

add_subdirectory(systems)
add_subdirectory(util)