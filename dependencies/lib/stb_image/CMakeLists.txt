cmake_minimum_required(VERSION 3.20)
project(stb_image)

if(MSVC)
 set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
# set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")


add_library(stb_image STATIC)
target_sources(stb_image PRIVATE src/stb_image.cpp)
target_include_directories(stb_image PUBLIC include)