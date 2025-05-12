cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    Core
    Simulation
    Strategies
)

file(GLOB_RECURSE SOURCES
    Core/*.cpp
    Simulation/*.cpp
    Strategies/*.cpp
)

set(MAIN_FILE BacktestEngine_Project.cpp)

add_executable(BacktestEngine ${MAIN_FILE} ${SOURCES})
