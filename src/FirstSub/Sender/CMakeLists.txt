cmake_minimum_required(VERSION 3.2)

project(Sender)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

find_package(libcluon REQUIRED)
include_directories(SYSTEM ${CLUON_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/Sender.cpp ${CMAKE_CURRENT_SOURCE_DIR}/NumModifier.cpp)
target_link_libraries(${PROJECT_NAME} ${CLUON_LIBRARIES})

enable_testing()
add_executable(${PROJECT_NAME}-Runner TestNumModifier.cpp ${CMAKE_CURRENT_SOURCE_DIR}/NumModifier.cpp)
add_test(NAME ${PROJECT_NAME}-Runner COMMAND ${PROJECT_NAME}-Runner)
