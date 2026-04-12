include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(googletest)

include_directories(${GTEST_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/src)

add_executable(
  ${PROJECT_NAME}_tests
  hello_world_test.cpp
)
target_link_libraries(
  ${PROJECT_NAME}_tests
  ${PROJECT_NAME}_lib
  GTest::gtest_main
  GTest::gmock
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)