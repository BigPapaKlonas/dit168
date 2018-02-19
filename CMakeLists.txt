cmake_minimum_required(VERSION 3.2)
project(dashFTab)
set(CMAKE_CXX_STANDARD 14)

add_library(vera MODULE ${CMAKE_SOURCE_DIR}/build/vera++-1.3.0/src)

file(GLOB_RECURSE srcs
  ${CMAKE_SOURCE_DIR}/src/*.cpp
  ${CMAKE_SOURCE_DIR}/src/*.h)
add_test(NAME VeraStyle
  COMMAND vera++
  --error
  ${srcs})