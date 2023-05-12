
--
-- If premake command is not supplied an action (target compiler), exit!
--
-- Targets of interest:
--     vs2019     (Visual Studio 2019)
--     gmake      (GNU Make)
--
if (_ACTION == nil) then
    return
end

workspace "FFT"
    -- destination directory for generated solution/project files
    location (_ACTION)

    -- destination directory for generated objects files
    objdir "../obj/%{cfg.buildcfg}-%{cfg.platform}"

    configurations { "Debug", "Release"}
    platforms { "x64" }
    architecture "x86_64"

    -- common debug configuration flags and symbols
    filter {"configurations:Debug"}
        targetsuffix "_d"
        defines { "DEBUG" }
        symbols "On"

    -- common release configuration flags and symbols
    filter {"configurations:Release"}
        defines { "NDEBUG" }
        optimize "On"

    -- C++ code in all projects
    systemversion "latest"
    language "C++"
    cppdialect "C++20"
    characterset "MBCS"

    project "WaveFileAnalysis"
        -- destination directory for compiled binary target
        targetdir ("../bin")
        kind "ConsoleApp"

        files { "../src/*.*" }

        includedirs { "../src/**" }
