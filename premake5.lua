workspace "Becketron"
	architecture "x64"
	targetdir "build"

	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	} 
	
	flags
	
	{
		"MultiProcessorCompile"
	}

	startproject "Sandbox"
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Becketron/vendor/GLFW/include"
IncludeDir["Glad"] = "Becketron/vendor/Glad/include"
IncludeDir["ImGui"] = "Becketron/vendor/imgui"
IncludeDir["glm"] = "Becketron/vendor/glm"
IncludeDir["stb_image"] = "Becketron/vendor/stb_image"
IncludeDir["entt"] = "Becketron/vendor/entt/include"
IncludeDir["ImFileBrowser"] = "Becketron/vendor/ImFileBrowser"
IncludeDir["PhysX"] = "Becketron/vendor/PhysX/include"

LibraryDir = {}
LibraryDir["PhysX"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysX_static_64.lib"
LibraryDir["PhysXCharacterKinematic"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysXCharacterKinematic_static_64.lib"
LibraryDir["PhysXCommon"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysXCommon_static_64.lib"
LibraryDir["PhysXCooking"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysXCooking_static_64.lib"
LibraryDir["PhysXExtensions"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysXExtensions_static_64.lib"
LibraryDir["PhysXFoundation"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysXFoundation_static_64.lib"
LibraryDir["PhysXPvd"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysXPvdSDK_static_64.lib"


group "Dependencies"
	include "Becketron/vendor/GLFW"
	include "Becketron/vendor/Glad"
	include "Becketron/vendor/imgui"
group ""

project "Becketron"
	location "Becketron"
	kind "StaticLib"
	language "C++"
	cppdialect "C++latest"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "btpch.h"
	pchsource "Becketron/src/btpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c", 
		"%{prj.name}/src/**.hpp", 
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/ImFileBrowser/**.cpp",
		"%{prj.name}/vendor/ImFileBrowser/**.h",
		"%{prj.name}/vendor/PhysX/include/**.cpp",
		"%{prj.name}/vendor/PhysX/include/**.h",
	}

--[[defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}--]]

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImFileBrowser}",
		"%{IncludeDir.PhysX}",
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
		"%{LibraryDir.PhysX}",
		"%{LibraryDir.PhysXCharacterKinematic}",
		"%{LibraryDir.PhysXCommon}",
		"%{LibraryDir.PhysXCooking}",
		"%{LibraryDir.PhysXExtensions}",
		"%{LibraryDir.PhysXFoundation}",
		"%{LibraryDir.PhysXPvd}"
	}

	defines
	{
		"PX_PHYSX_STATIC_LIB"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"BT_PLATFORM_WINDOWS",
			"BTRON_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}


	filter "configurations:Debug"
		defines "BT_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines 
		{
			"BT_RELEASE",
			"NDEBUG" -- PhysX Requires This
		}
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "BT_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++latest"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	links
	{
		"Becketron"
	}

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c", 
		"%{prj.name}/src/**.hpp", 
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"Becketron/vendor/spdlog/include",
		"%{prj.name}/src",
		"Becketron/src",
		"Becketron/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.PhysX}"
	}


	filter "system:windows"
		systemversion "latest"

		defines
		{
			"BT_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "BT_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "BT_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "BT_DIST"
		runtime "Release"
		optimize "on"


project "Tron"
		location "Tron"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++latest"
		staticruntime "on"
		
		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
			"%{prj.name}/assets/**.glsl",
		}
		
		includedirs
		{
			"%{prj.name}/src",
			"Becketron/src",
			"Becketron/vendor",
			"Becketron/vendor/spdlog/include",
			"%{IncludeDir.glm}",
			"%{IncludeDir.entt}",
			"%{IncludeDir.ImFileBrowser}",
			"%{IncludeDir.PhysX}"
		}
	
		links
		{
			"Becketron"
		}
	
		filter "system:windows"
			systemversion "latest"
	
			defines
			{
				"BT_PLATFORM_WINDOWS"
			}
	
		filter "configurations:Debug"
			defines "BT_DEBUG"
			runtime "Debug"
			symbols "on"
	
		filter "configurations:Release"
			defines "BT_RELEASE"
			runtime "Release"
			optimize "on"
	
		filter "configurations:Dist"
			defines "BT_DIST"
			runtime "Release"
			optimize "on"