target_sources(
  ${PROJECT_NAME}_lib
  PRIVATE
  player_projectile.cpp
  player.cpp
  test_scene.cpp
)

add_subdirectory(systems)