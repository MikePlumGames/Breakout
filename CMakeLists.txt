cmake_minimum_required(VERSION 3.20)

# 1.0. project declaration
project(Breakout LANGUAGES C CXX)

# 1.1. c++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#! ! ! ! ! ! !
# Set this to ON instead of OFF to ship the game!
#! ! ! ! ! ! !
set(PRODUCTION_BUILD OFF CACHE BOOL "Make this a production build" FORCE)

# 2.0. multi-threaded static runtime string (/MT and /MTd) - staticly compile c++ runtime
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# 3.0. production optimization in production build
if(PRODUCTION_BUILD)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) 
else()
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION FALSE)
endif()

# 3.1. SIMD Performance Accelerations
if(MSVC) 
    add_compile_options(/arch:AVX2) 
endif()

# 4.0. global third-party library build control flags
# 4.1 glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
# 4.2 global
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
# 4.3 freetype
set(FT_DISABLE_ZLIB ON CACHE INTERNAL "")
set(FT_DISABLE_BZIP2 ON CACHE INTERNAL "")
set(FT_DISABLE_PNG ON CACHE INTERNAL "")
set(FT_DISABLE_HARFBUZZ ON CACHE INTERNAL "")

# 5.0. add subdirectories from /dependencies/lib/ - all libraries containing c/c++ and h files
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib/glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib/miniaudio)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib/stb_image)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib/glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib/freetype)

# 6.0. get all source files
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

# 6.1. register .rc file on windows for app icon
if(WIN32)
    list(APPEND MY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/appIcon.rc")
endif()

# 7.0. add an executable
add_executable("${CMAKE_PROJECT_NAME}" ${MY_SOURCES})

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC GLFW_INCLUDE_NONE=1) 

# 8.0. set preprocessor macros in c++ to check if we are in production or development build
if(PRODUCTION_BUILD)
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1) 
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DEVELOPMENT_BUILD=0) 
else()
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0) 
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DEVELOPMENT_BUILD=1) 
endif()

# 9.0. windows compiler flags
if(MSVC)
    # 9.1. hide unnecessary warnings
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)
    # 9.2. Hides the console window on windows in releasemode
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
        # 9.2.1. open the console window in Debug
        target_link_options("${CMAKE_PROJECT_NAME}" PRIVATE "/SUBSYSTEM:CONSOLE")
    else()
        # 9.2.2. close the console window in Release, RelWithDebInfo, MinSizeRel, etc.
        target_link_options("${CMAKE_PROJECT_NAME}" PRIVATE "/SUBSYSTEM:WINDOWS" "/ENTRY:mainCRTStartup")
    endif()
endif()
# 9.3. make windows use char not wchar_t for its functions
if (WIN32)
    target_compile_options("${CMAKE_PROJECT_NAME}" PRIVATE /UUNICODE /U_UNICODE)
endif()

# 10.0. add include directories
# the src folder with game files(.cpp and .h) and dependencies/include with header-only libraries
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/include/")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/")

# 11.0. os specific graphics bindings lookup
if(WIN32)   # 11.1. windows
    set(OS_GRAPHICS_LIBS opengl32)
elseif(APPLE) # 11.2. macOS
    find_library(OPENGL_FRAMEWORK OpenGL REQUIRED)
    find_library(COCOA_FRAMEWORK Cocoa REQUIRED)
    find_library(IOKIT_FRAMEWORK IOKit REQUIRED)
    find_library(COREVIDEO_FRAMEWORK CoreVideo REQUIRED)
    set(OS_GRAPHICS_LIBS ${OPENGL_FRAMEWORK} ${COCOA_FRAMEWORK} ${IOKIT_FRAMEWORK} ${COREVIDEO_FRAMEWORK})
else() # 11.3. linux/other
    set(OS_GRAPHICS_LIBS GL m pthread dl)
endif()

# 12.0. add the libraries from step 5.0.
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE 
    glad 
    miniaudio 
    stb_image
    glfw 
    freetype
    ${OS_GRAPHICS_LIBS}
)

# 13.0 copy the assets folder next to the .exe
add_custom_command(
    TARGET "${CMAKE_PROJECT_NAME}" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/assets"
    COMMENT "Copying game assets folder to runtime build target directory..."
)
