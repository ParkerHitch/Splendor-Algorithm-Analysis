cmake_minimum_required(VERSION 3.23)
project(Research)

set(CMAKE_CXX_STANDARD 20)

add_executable(Research src/main.cpp src/game.h src/game.cpp src/gameState.h src/Display/gameOutput.cpp src/Display/gameOutput.h src/Display/consoleColors.h src/Players/HumanPlayer.cpp src/Players/Player.h src/gameSetup.cpp src/gameSetup.h src/gameAction.h)
