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

project "Hazel"
	location "Hazel"
   	kind "SharedLib"
   	language "C++"
   	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
   	objdir ("intermediate/" .. outputdir .. "/%{prj.name}")

   	files { 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp" 
	}

	includedirs {
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL"
		}

		-- Links DLL
		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

   	filter "configurations:Debug"
      	defines { 
			"HZ_DEBUG" 
		}
      	symbols "On"

   	filter "configurations:Release"
      	defines { 
			"HZ_RELEASE" 
		}
		optimize "On"

	filter "configurations:Dist"
      	defines { 
			"HZ_DIST" 
		}
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
		staticruntime "On"
		systemversion "latest"

		defines {
			"HZ_PLATFORM_WINDOWS"
		}

   	filter "configurations:Debug"
      	defines { 
			"HZ_DEBUG" 
		}
      	symbols "On"

   	filter "configurations:Release"
      	defines { 
			"HZ_RELEASE" 
		}
		optimize "On"

	filter "configurations:Dist"
      	defines { 
			"HZ_DIST" 
		}
		optimize "On"
	   