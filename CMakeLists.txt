cmake_minimum_required(VERSION 3.20.0)
project(askc)

add_subdirectory(src/settings)

include_directories(include)

add_executable(askc src/askc.cpp)

# Link the settings library to the askc executable
target_link_libraries(askc PRIVATE settings)
