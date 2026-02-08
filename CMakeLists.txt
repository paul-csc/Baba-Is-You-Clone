cmake_minimum_required(VERSION 3.20)

project(BabaIsYou
    LANGUAGES CXX
)

# ---- Global settings ----
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Sources ----
file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.h
    src/*.hpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

set(RAYLIB_ROOT "C:/raylib-5.5")

add_library(raylib STATIC IMPORTED)

set_target_properties(raylib PROPERTIES
    IMPORTED_LOCATION "${RAYLIB_ROOT}/lib/raylib.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${RAYLIB_ROOT}/include"
)

target_include_directories(${PROJECT_NAME} PRIVATE
    src
    include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    raylib
    winmm
    gdi32
    user32
    opengl32
    kernel32
)
