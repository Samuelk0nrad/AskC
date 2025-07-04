
cmake_minimum_required(VERSION 3.20.0)
project(askc)

set(CLIP_EXAMPLES OFF CACHE BOOL "Compile clip examples")
set(CLIP_TESTS OFF CACHE BOOL "Compile clip tests")

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG da40186618909b1a7363d4e4495aa899c6e0eb75)
FetchContent_MakeAvailable(cpr)

add_subdirectory(src/settings)
add_subdirectory(clip) # <--- Add this line

include_directories(include)
include_directories(clip)  # <--- Add this line to include clip headers

add_executable(askc src/askc.cpp)

target_link_libraries(askc PRIVATE cpr::cpr settings clip) # <--- Use 'clip', not a path

