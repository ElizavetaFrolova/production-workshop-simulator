cmake_minimum_required(VERSION 3.16)

project(production_workshop_simulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(simulator_core
    src/parser/Parser.cpp
    src/simulation/Simulator.cpp
    src/output/OutputWriter.cpp
)

target_include_directories(simulator_core PUBLIC
    include
)

add_executable(simulator
    src/main.cpp
)

target_link_libraries(simulator PRIVATE
    simulator_core)

enable_testing()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(googletest)
add_subdirectory(tests)