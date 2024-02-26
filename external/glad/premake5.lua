project "GLAD"
	kind "StaticLib"
	language "C"

	targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/glad/*.h",
        "src/*.c"
	}

    includedirs
    {
        "include"
    }

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		defines
		{
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
