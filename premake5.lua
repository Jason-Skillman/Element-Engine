-- premake5.lua
workspace "Element-Engine"
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
IncludeDir["ElementSrc"] = "Element/src"
IncludeDir["spdlog"] = "Element/vendor/spdlog/include"
IncludeDir["GLFW"] = "Element/vendor/GLFW/include"
IncludeDir["Glad"] = "Element/vendor/Glad/include"
IncludeDir["ImGui"] = "Element/vendor/ImGui"
IncludeDir["glm"] = "Element/vendor/glm"
IncludeDir["stb"] = "Element/vendor/stb"

-- Includes other project premake files
group "Dependencies"
	include "Element/vendor/GLFW"
	include "Element/vendor/Glad"
	include "Element/vendor/ImGui"
group ""

project "Element"
	location "Element"
   	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("intermediate/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "Element/src/pch.cpp"

   	files { 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb/**.h",
		"%{prj.name}/vendor/stb/**.cpp"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"HZ_PROFILE"
	}

	includedirs {
		"%{IncludeDir.ElementSrc}",
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
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/assets/**"
	}

	defines {
		"HZ_PROFILE"
	}

	includedirs {
		"Sandbox/src",
		"%{IncludeDir.ElementSrc}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}"
	}

	links {
		"Element"
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
