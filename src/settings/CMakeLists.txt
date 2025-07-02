add_library(settings settings.cpp)

target_include_directories(settings PUBLIC $CMAKE_CURRENT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/include )
