cmake_minimum_required(VERSION 3.2)
project(dashFTab)
set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Treat warnings as errors
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

# Use Vera for styling checks
find_package(vera++)

# Use Valgrind for memory checks
find_program( MEMORYCHECK_COMMAND valgrind )
set( MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full" )

file(GLOB_RECURSE srcs
  ${CMAKE_SOURCE_DIR}/src/*.cpp
  ${CMAKE_SOURCE_DIR}/src/*.h)
add_test(NAME VeraStyle
  COMMAND vera++
  --error
  ${srcs})
