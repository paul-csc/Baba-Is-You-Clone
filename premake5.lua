workspace "Sokoban"
    architecture "x64"
    location "."
    
    configurations
    {
        "Debug",
        "Release"
    }

project "Sokoban"
    language "C++"
    cppdialect "C++20"
    staticruntime "Off"

    files
    {
        "src/**.h",
        "src/**.hpp",
        "src/**.cpp"
    }

    includedirs
    {
        "src",
        "dependencies/raylib-5.5/include"
    }

    filter "system:windows"
        systemversion "latest"
    filter {}

    filter "toolset:gcc"
        libdirs { "dependencies/raylib-5.5/lib" }
        links { ":libraylib.a", "winmm", "gdi32", "user32", "opengl32", "kernel32" }
    filter {}
        
    filter "configurations:Debug"
        defines { "DEBUG" }
        kind "ConsoleApp"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        kind "WindowedApp"
        runtime "Release"
        optimize "On"
        symbols "Off"