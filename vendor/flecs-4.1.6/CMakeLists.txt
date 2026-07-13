add_library(flecs-4.1.6 STATIC 
    flecs.c
    flecs.h
)
target_include_directories(flecs-4.1.6 PUBLIC .)
target_compile_features(flecs-4.1.6 PUBLIC c_std_99)

find_package(Threads REQUIRED)
target_link_libraries(flecs-4.1.6 PRIVATE Threads::Threads)