cmake_minimum_required(VERSION 3.20)
project(glad)

if(MSVC)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
# set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")


add_library(glad STATIC)
target_sources(glad PRIVATE src/glad.c)
target_include_directories(glad PUBLIC include)