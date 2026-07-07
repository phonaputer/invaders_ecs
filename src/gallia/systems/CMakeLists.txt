target_sources(
  ${PROJECT_NAME}_lib
  PRIVATE
  animation.cpp
  collision_detection.cpp
  deletion.cpp
  lifetime.cpp
  position_following.cpp
  rendering.cpp
  velocity.cpp
)

add_subdirectory(debug)
add_subdirectory(invaders)
add_subdirectory(player)