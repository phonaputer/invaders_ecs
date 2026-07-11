include(FetchContent)

FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-3.4.2
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(SDL3)

add_library(
  ${PROJECT_NAME}_lib
)
target_link_libraries(${PROJECT_NAME}_lib PRIVATE SDL3::SDL3)
target_include_directories(${PROJECT_NAME}_lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(framework)
add_subdirectory(game)

add_executable(
  ${PROJECT_NAME} 
  main.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 ${PROJECT_NAME}_lib)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")