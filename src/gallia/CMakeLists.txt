target_sources(
  ${PROJECT_NAME}_lib
  PRIVATE
  hud.cpp
  invaders.cpp
  player_projectile.cpp
  player.cpp
  test_scene.cpp
)

add_subdirectory(systems)
add_subdirectory(util)