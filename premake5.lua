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
IncludeDir["GLFW"] = "Hazel/vendor/GLFW/include"
IncludeDir["Glad"] = "Hazel/vendor/Glad/include"
IncludeDir["ImGui"] = "Hazel/vendor/ImGui"

-- Includes the GLFW premake file
include "Hazel/vendor/GLFW"
include "Hazel/vendor/Glad"
include "Hazel/vendor/ImGui"

project "Hazel"
	location "Hazel"
   	kind "SharedLib"
   	language "C++"
   	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("intermediate/" .. outputdir .. "/%{prj.name}")
	staticruntime "off"
	   
	pchheader "hzpch.h"
	pchsource "Hazel/src/hzpch.cpp"

   	files { 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp" 
	}

	includedirs {
		"%{prj.name}/src/",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links {
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines {
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		-- Links DLL
		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

   	filter "configurations:Debug"
      	defines { 
			"HZ_DEBUG"
		}
		runtime "Debug"
      	symbols "On"

   	filter "configurations:Release"
      	defines { 
			"HZ_RELEASE" 
		}
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
      	defines { 
			"HZ_DIST" 
		}
		runtime "Release"
		optimize "On"

	--[[
	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"
	]]--

project "Sandbox"
	location "Sandbox"
   	kind "ConsoleApp"
   	language "C++"
   	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("intermediate/" .. outputdir .. "/%{prj.name}")
	staticruntime "off"

	files { 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp" 
	}

	includedirs {
		"Hazel/src",
		"Hazel/vendor/spdlog/include"
	}

	links {
		"Hazel"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines {
			"HZ_PLATFORM_WINDOWS"
		}

   	filter "configurations:Debug"
      	defines { 
			"HZ_DEBUG" 
		}
		runtime "Debug"
      	symbols "On"

   	filter "configurations:Release"
      	defines { 
			"HZ_RELEASE" 
		}
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
      	defines { 
			"HZ_DIST" 
		}
		runtime "Release"
		optimize "On"
	   