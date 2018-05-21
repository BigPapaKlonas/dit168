# BASED ON https://github.com/chalmers-revere/opendlv-device-ultrasonic-srf08
# by Chalmers-Revere

cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 14)
project(ultrasonic)

# Set constants
set(MESSAGES messages.odvd)
set(CLUON_COMPLETE cluon-complete.hpp)
# Make sure cluon-complete, messages.odvd and the .cpp files are in the microservices src folder (not root)
# or you will need to change the pathing for the custom commands

# Require pthreads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Extract cluon-msc from cluon-complete.hpp.
add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/cluon-msc
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/src/${CLUON_COMPLETE} ${CMAKE_BINARY_DIR}/cluon-complete.hpp
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_BINARY_DIR}/cluon-complete.hpp ${CMAKE_BINARY_DIR}/cluon-complete.cpp
    COMMAND ${CMAKE_CXX_COMPILER} -o ${CMAKE_BINARY_DIR}/cluon-msc ${CMAKE_BINARY_DIR}/cluon-complete.cpp -std=c++14 -pthread -D HAVE_CLUON_MSC
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/${CLUON_COMPLETE}) 

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/messages.cpp
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_BINARY_DIR}/cluon-msc --cpp-sources --cpp-add-include-file=messages.hpp --out=${CMAKE_BINARY_DIR}/messages.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/${MESSAGES}
    COMMAND ${CMAKE_BINARY_DIR}/cluon-msc --cpp-headers --out=${CMAKE_BINARY_DIR}/messages.hpp ${CMAKE_CURRENT_SOURCE_DIR}/src/${MESSAGES}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/${MESSAGES} ${CMAKE_BINARY_DIR}/cluon-msc)

# Add current build directory as include directory as it contains generated files.
include_directories(SYSTEM ${CMAKE_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Gather all object code first to avoid double compilation.
add_library(${PROJECT_NAME}-core OBJECT  ${CMAKE_BINARY_DIR}/messages.cpp)
set(LIBRARIES Threads::Threads ${CURSES_LIBRARIES})

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/ultrasonic.cpp ${CMAKE_BINARY_DIR}/messages.cpp)
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

