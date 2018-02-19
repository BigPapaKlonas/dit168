cmake_minimum_required(VERSION 3.2)
project(dashFTab)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/modules)

find_package(vera++)
find_package(valgrind)
find_package(lcov)
find_package(flawfinder)
file(GLOB_RECURSE srcs
  ${CMAKE_SOURCE_DIR}/src/*.cpp
  ${CMAKE_SOURCE_DIR}/src/*.h)
add_test(NAME VeraStyle
  COMMAND vera++
  --error
  ${srcs})
