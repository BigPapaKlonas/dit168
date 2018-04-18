cmake_minimum_required(VERSION 3.2)
project(maneuvering)
set(CMAKE_CXX_STANDARD 14)

find_package(libcluon REQUIRED)
include_directories(SYSTEM ${CLUON_INCLUDE_DIRS})

###########################################################################
# Auto generate message cpp & hpp from .odvd message file
add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/messages.cpp
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND cluon-msc --cpp-sources --cpp-add-include-file=messages.hpp --out=${CMAKE_BINARY_DIR}/messages.cpp ${CMAKE_CURRENT_SOURCE_DIR}/messages.odvd
        COMMAND cluon-msc --cpp-headers --out=${CMAKE_BINARY_DIR}/messages.hpp ${CMAKE_CURRENT_SOURCE_DIR}/messages.odvd
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/messages.odvd)
include_directories(SYSTEM ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/maneuvering.cpp ${CMAKE_BINARY_DIR}/messages.cpp)
target_link_libraries(${PROJECT_NAME} ${CLUON_LIBRARIES})


###########################################################################
# Teesting tool-chain
enable_testing()

# Treat warnings as errors
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

# Files to test
file(GLOB_RECURSE src_to_check
        ${CMAKE_CURRENT_SOURCE_DIR}/maneuvering.cpp
        )

### TESTS ### To see the test logs, navigate to build and run 'ctest -VV'
# Set up vera++ test - used for styling checks
# add_test(
#  NAME vera++
#  COMMAND vera++
#  --show-rule
#  --error
#  ${src_to_check}
# )

# Set up cppcheck test - used to detect bugs and dangerous code constructs
#add_test(
#        NAME cppcheck
#        COMMAND /usr/bin/cppcheck
#        --enable=warning,performance,portability,information,missingInclude
#        --error-exitcode=1
#        --inconclusive
#        --std=c++11
#        --library=qt.cfg
#        --quiet
#        ${src_to_check}
#)
