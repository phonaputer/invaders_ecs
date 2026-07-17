target_sources(
  ${PROJECT_NAME}_lib
  PRIVATE
  fortress.cpp
  hud.cpp
  invaders.cpp
  player.cpp
  scene.cpp
)

add_subdirectory(systems)