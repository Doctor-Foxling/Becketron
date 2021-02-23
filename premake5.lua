workspace "Becketron"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	} 

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Becketron/vendor/GLFW/include"
IncludeDir["Glad"] = "Becketron/vendor/Glad/include"
IncludeDir["ImGui"] = "Becketron/vendor/imgui"
IncludeDir["glm"] = "Becketron/vendor/glm"
IncludeDir["stb_image"] = "Becketron/vendor/stb_image"

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
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
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
		defines "BT_RELEASE"
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

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"Becketron/vendor/spdlog/include",
		"Becketron/src",
		"Becketron/vendor",
		"%{IncludeDir.glm}"
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