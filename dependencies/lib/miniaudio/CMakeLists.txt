cmake_minimum_required(VERSION 3.20)
project(miniaudio)

if(MSVC)
 set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
# set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")


add_library(miniaudio STATIC)
target_sources(miniaudio PRIVATE src/miniaudio.cpp)
target_include_directories(miniaudio PUBLIC include)