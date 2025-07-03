cmake_minimum_required(VERSION 3.20.0)
project(askc)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG da40186618909b1a7363d4e4495aa899c6e0eb75) # Replace with your desired git commit from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

add_subdirectory(src/settings)

include_directories(include)

add_executable(askc src/askc.cpp)

# Link the settings library to the askc executable
target_link_libraries(askc PRIVATE cpr::cpr settings)
