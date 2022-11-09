cmake_minimum_required(VERSION 3.23)
project(Research)

set(CMAKE_CXX_STANDARD 20)

add_executable(Research src/main.cpp src/game.h src/game.cpp src/gameState.cpp src/gameState.h src/gameEvents.cpp src/gameEvents.h)
