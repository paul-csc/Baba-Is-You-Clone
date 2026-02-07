cmake_minimum_required(VERSION 3.20)

project(BabaIsYou
    LANGUAGES CXX
)

# ---- Global settings ----
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# x64
if (NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "This project requires a 64-bit build")
endif()

# ---- Sources ----
file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.h
    src/*.hpp
)

add_executable(BabaIsYou ${SOURCES})

# ---- Include directories ----
target_include_directories(BabaIsYou PRIVATE
    src
    dependencies/raylib-5.5/include
)

target_link_directories(BabaIsYou PRIVATE
    dependencies/raylib-5.5/lib
)

target_link_libraries(BabaIsYou PRIVATE
    raylib
    winmm
    gdi32
    user32
    opengl32
    kernel32
)

# ---- Windows-specific settings ----
if (WIN32)
    set_target_properties(BabaIsYou PROPERTIES
        WIN32_EXECUTABLE $<CONFIG:Release>
    )
endif()

# ---- Configurations ----
target_compile_definitions(BabaIsYou PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:RELEASE>
)
