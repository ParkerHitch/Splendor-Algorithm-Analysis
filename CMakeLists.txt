cmake_minimum_required(VERSION 3.23)
project(Research)

set(CMAKE_CXX_STANDARD 20)

add_executable(Research src/main.cpp src/game/game.h src/game/game.cpp src/game/gameState.h src/display/gameOutput.cpp src/display/gameOutput.h src/display/consoleColors.h src/players/HumanPlayer.cpp src/players/Player.h src/game/gameSetup.cpp src/game/gameSetup.h src/game/gameAction.h src/players/agents/RandomPlayer.cpp src/game/gameAction.cpp src/utils/config.h)
