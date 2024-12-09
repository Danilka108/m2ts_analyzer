cmake_minimum_required(VERSION 3.5)
project(mpegts_analyzer)

set(CMAKE_CXX_STANDART 11)
set(CMAKE_CXX_STANDART_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -L/lib/x86_64-linux-gnu/")
# set(CMAKE_C_COMPILER "/usr/lib/clang")
# set(CMAKE_CXX_COMPILER "/usr/lib/clang++")

# SET (CMAKE_CXX_FLAGS_INIT                "-Wall -std=c++11")
# SET (CMAKE_CXX_FLAGS_DEBUG_INIT          "-g")
# SET (CMAKE_CXX_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
# SET (CMAKE_CXX_FLAGS_RELEASE_INIT        "-O3 -DNDEBUG")
# SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

add_executable(m2ts_analyzer ${SOURCES})

