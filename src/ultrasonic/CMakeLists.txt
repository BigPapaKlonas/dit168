cmake_minimum_required(VERSION 3.0.2)
project(ultrasonic)
set(CMAKE_CXX_STANDARD 14)

#include_directories(SYSTEM ${CLUON_INCLUDE_DIRS})

#add_library(libcluon STATIC cluon-complete.hpp)
#set_target_properties(libcluon PROPERTIES CXX_STANDARD 14 LINKER_LANGUAGE CXX)

add_library(libcluon INTERFACE)
target_include_directories(libcluon INTERFACE ${./CMAKE_SOURCE_DIR})

install(TARGETS libcluon EXPORT libcluon DESTINATION include)
install(EXPORT libcluon DESTINATION libcluon)

find_package(libcluon REQUIRED ${CMAKE_BINARY_DIR}/CMakeFiles/Export/libcluon)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/messages.cpp
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND cluon-msc --cpp-sources --cpp-add-include-file=messages.hpp --out=${CMAKE_BINARY_DIR}/messages.cpp ${CMAKE_CURRENT_SOURCE_DIR}/messages.odvd

        COMMAND cluon-msc --cpp-headers --out=${CMAKE_BINARY_DIR}/messages.hpp ${CMAKE_CURRENT_SOURCE_DIR}/messages.odvd

       DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/messages.odvd)
include_directories(SYSTEM ${CMAKE_BINARY_DIR})
include_directories(SYSTEM ${CMAKE_BINARY_DIR}/CMakeFiles/Export/libcluon)

#link_directories(/etc/roboticscape)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${CMAKE_BINARY_DIR}/messages.cpp)

#target_link_libraries(${PROJECT_NAME} roboticscape)


