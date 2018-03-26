cmake_minimum_required(VERSION 3.2)
project(ultrasonic_cpp)
set(CMAKE_CXX_STANDARD 14)

link_directories(/etc/roboticscape)

add_executable(ultrasonic_cpp main.cpp)

target_link_libraries(ultrasonic_cpp robitcscape)