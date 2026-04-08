add_subdirectory(component)
add_subdirectory(system)

target_sources(
  ${PROJECT_NAME}
  PRIVATE
  entity_manager.cpp
)