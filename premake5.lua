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

-- Vulkan
VULKAN_SDK = os.getenv("VULKAN_SDK")

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
IncludeDir["shaderc"] = "Element/vendor/shaderc/include"
IncludeDir["SPIRV_Cross"] = "Element/vendor/SPIRV-Cross"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] = "%{wks.location}/Element/vendor/VulkanSDK/Lib"
LibraryDir["VulkanSDK_DebugDLL"] = "%{wks.location}Element/vendor/VulkanSDK/Bin"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"

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
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.VulkanSDK}"
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

		links {
			"%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}"
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

		defines { 
			"RELEASE"
		}

		links {
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
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

		postbuildcommands {
			"xcopy \"%{LibraryDir.VulkanSDK_DebugDLL}\" \"%{cfg.targetdir}\" /Y"
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
