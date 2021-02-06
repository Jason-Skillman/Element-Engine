-- premake5.lua
workspace "Hazel-Engine"
	architecture "x64"

	configurations { 
		"Debug", 
		"Release",
		"Dist"
	}

	startproject "Sandbox"

-- bin/Windows/Debug/x64
outputdir = "%{cfg.system}/%{cfg.buildcfg}/%{cfg.architecture}"

-- Include dir relative to root folder path (solution dir)
IncludeDir = {}
IncludeDir["HazelSrc"] = "Hazel/src"
IncludeDir["spdlog"] = "Hazel/vendor/spdlog/include"
IncludeDir["GLFW"] = "Hazel/vendor/GLFW/include"
IncludeDir["Glad"] = "Hazel/vendor/Glad/include"
IncludeDir["ImGui"] = "Hazel/vendor/ImGui"
IncludeDir["glm"] = "Hazel/vendor/glm"
IncludeDir["stb"] = "Hazel/vendor/stb"

-- Includes other project premake files
group "Dependencies"
	include "Hazel/vendor/GLFW"
	include "Hazel/vendor/Glad"
	include "Hazel/vendor/ImGui"
group ""

project "Hazel"
	location "Hazel"
   	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("intermediate/" .. outputdir .. "/%{prj.name}")

	pchheader "hzpch.h"
	pchsource "Hazel/src/hzpch.cpp"

   	files { 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb/**.h",
		"%{prj.name}/vendor/stb/**.cpp"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs {
		"%{IncludeDir.HazelSrc}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}"
	}

	links {
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

		defines { 
			"HZ_DEBUG"
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

		defines { 
			"HZ_RELEASE" 
		}

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"

		defines { 
			"HZ_DIST" 
		}

project "Sandbox"
	location "Sandbox"
   	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
 	objdir ("intermediate/" .. outputdir .. "/%{prj.name}")

	files { 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp" 
	}

	includedirs {
		"%{IncludeDir.HazelSrc}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}"
	}

	links {
		"Hazel"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"HZ_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

		defines { 
			"HZ_DEBUG" 
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

		defines { 
			"HZ_RELEASE" 
		}

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"

		defines { 
			"HZ_DIST" 
		}
