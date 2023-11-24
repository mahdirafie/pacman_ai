cmake_minimum_required(VERSION 3.12)
project(Pacman)
set(CMAKE_CXX_STANDARD 11)
set(sources
	src/main.cpp
	src/tile.cpp
	#src/game.cpp
	src/map.cpp
	src/wall.cpp
	src/pacman.cpp
	src/ghost.cpp
	src/blinky.cpp
	src/pinky.cpp
	src/inky.cpp
	src/clyde.cpp
	src/chase.cpp
	src/scatter.cpp
	src/eaten.cpp
	src/timer.cpp
	src/pellet.cpp
	src/frightened.cpp
	src/animation.cpp
	src/menu.cpp
	src/Game.cpp
	src/fruit.cpp
	)
include_directories(header)
add_executable(exe ${sources})
target_link_libraries(exe sfml-graphics sfml-window sfml-audio sfml-system)