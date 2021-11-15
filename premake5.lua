-- premake5.lua
workspace "Element-Engine"
	architecture "x64"

	configurations { 
		"Debug", 
		"Release",
	}

	startproject "ElementEditor"

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
IncludeDir["entt"] = "Element/vendor/entt/include"
IncludeDir["yaml_cpp"] = "Element/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "Element/vendor/ImGuizmo"

-- Includes other project premake files
group "Dependencies"
	include "Element/vendor/GLFW"
	include "Element/vendor/Glad"
	include "Element/vendor/ImGui"
	include "Element/vendor/yaml-cpp"
group ""

project "Element"
	location "Element"
   	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("intermediate/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "Element/src/pch.cpp"

   	files { 
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb/**.h",
		"%{prj.name}/vendor/stb/**.cpp",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"PROFILE"
	}

	includedirs {
		"%{IncludeDir.ElementSrc}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}"
	}

	links {
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib"
	}

	-- Marks ImGuizmo files to not require the pch file as it is not needed.
	filter "files:Element/vendor/ImGuizmo/**.cpp"
		flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines {
			"PLATFORM_WINDOWS",
			"BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

		defines { 
			"DEBUG"
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

		defines { 
			"RELEASE" 
		}

project "ElementEditor"
		location "ElementEditor"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		staticruntime "off"
		
		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("intermediate/" .. outputdir .. "/%{prj.name}")
	
		files { 
			"%{prj.name}/src/**.h", 
			"%{prj.name}/src/**.cpp",
			"%{prj.name}/assets/**"
		}
		
		includedirs {
			"ElementEditor/src",
			"%{IncludeDir.ElementSrc}",
			"%{IncludeDir.spdlog}",
			"%{IncludeDir.glm}",
			"%{IncludeDir.ImGui}",
			"%{IncludeDir.entt}",
			"%{IncludeDir.ImGuizmo}"
		}
	
		links {
			"Element"
		}
	
		filter "system:windows"
			systemversion "latest"
	
			defines {
				"PLATFORM_WINDOWS"
			}
	
		filter "configurations:Debug"
			runtime "Debug"
			symbols "on"
	
			defines { 
				"DEBUG" 
			}
	
		filter "configurations:Release"
			runtime "Release"
			optimize "on"
	
			defines { 
				"RELEASE" 
			}

project "Sandbox"
	location "Sandbox"
   	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
 	objdir ("intermediate/" .. outputdir .. "/%{prj.name}")

	files { 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/assets/**"
	}

	defines {
		"PROFILE"
	}

	includedirs {
		"Sandbox/src",
		"%{IncludeDir.ElementSrc}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.entt}"
	}

	links {
		"Element"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

		defines { 
			"DEBUG" 
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

		defines { 
			"RELEASE" 
		}
