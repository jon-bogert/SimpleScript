workspace "SimpleScript"
architecture "x64"
    configurations { "Debug", "Release" }
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Core"
    location "%{prj.name}"
    kind "WindowedApp"
    language "C++"
    targetname "%{prj.name}"
    targetdir ("bin/".. outputdir)
    objdir ("%{prj.name}/int/" .. outputdir)
    cppdialect "C++17"
    staticruntime "Off"

    defines { "YAML_CPP_STATIC_DEFINE" }

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.c",
        "%{prj.name}/**.hpp"
,        "%{prj.name}/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/include",
        "%{prj.name}/include/imgui",
        "%{prj.name}/include/minidocx",
        "%{prj.name}/src",
    }

    libdirs "%{prj.name}/lib"

    links
    {
        "imgui",
        "d3d11",
        "d3dcompiler",
        "dxgi",
        "shell32",
    }

    filter "system:windows"
		systemversion "latest"
		defines { "WIN32" }

	filter "configurations:Debug"
		defines { "_DEBUG", "_CONSOLE" }
		symbols "On"
        links
        {
            "imgui-d",
            "minidocx-d",
			"XephInput-d",
            "yaml-cpp-d"
        }

    filter "configurations:Release"
		defines { "NDEBUG", "NCONSOLE" }
		optimize "On"
        links
        {
            "imgui",
            "minidocx",
			"XephInput",
            "yaml-cpp"
        }