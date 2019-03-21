cmake_minimum_required(VERSION 3.10)
project(insurance)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} sources)
