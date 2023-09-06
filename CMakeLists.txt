cmake_minimum_required(VERSION 3.12)
project(chip8_emulator)

set(CMAKE_CXX_STANDARD 14) # C++ VERSION

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})

add_executable(chip8 src/main.cpp)

target_link_libraries(chip8 ${SDL2_LIBRARIES} ${GLUT_LIBRARIES})