cmake_minimum_required(VERSION 3.2)
project(dashFTab)
set(CMAKE_CXX_STANDARD 14)

# Link to cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")



add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/FirstSub/Communicator)
#These subdirectories should be used after the first hand-in.
#add_subdirectory(test)
#add_subdirectory(src)
enable_testing()
##add_test(NAME RootTest COMMAND Test)

# Treat warnings as errors
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

# Use for Unit testing
find_package(cppunit)

# Use to detect bugs and dangerous code constructs
find_package(cppcheck)

# Use to autogenerate docs
find_package(Doxygen)

# Use for styling checks
find_package(vera++)

# Use for memory checks
find_program( MEMORYCHECK_COMMAND Valgrind )
set( MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full" )

# Set up vera tests
file(GLOB_RECURSE srcs
  ${CMAKE_SOURCE_DIR}/src/*.cpp
  ${CMAKE_SOURCE_DIR}/src/*.h)
add_test(NAME VeraStyle
  COMMAND vera++
  --error
  ${srcs})
