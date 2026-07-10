target_sources(
  ${PROJECT_NAME}_lib
  PRIVATE
  animation.cpp
  collision_detection.cpp
  damage.cpp
  deletion.cpp
  hud_rendering.cpp
  lifetime.cpp
  position_following.cpp
  rendering.cpp
  score.cpp
  velocity.cpp
)

add_subdirectory(debug)
add_subdirectory(invaders)
add_subdirectory(player)