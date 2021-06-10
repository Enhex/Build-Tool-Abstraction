newoption {
	trigger     = "location",
	value       = "./",
	description = "Where to generate the project.",
}

if not _OPTIONS["location"] then
	_OPTIONS["location"] = "./"
end

newoption {
	trigger     = "mingw",
	description = "MinGW is being used to cross compile.",
}

include(_OPTIONS["location"] .. "conanbuildinfo.premake.lua")

workspace("Build Tool Abstraction")
	location(_OPTIONS["location"])
	conan_basic_setup()

	project("build")
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		targetdir = _OPTIONS["location"] .. "bin/%{cfg.buildcfg}"

		defines{"_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS"}

		files{
			"src/*",
		}

		-- needed to cross compile
		if _OPTIONS["mingw"] then
			libdirs {"/usr/x86_64-w64-mingw32/lib"}
			links{"ws2_32"}
		end

		filter "configurations:Debug"
			defines { "DEBUG" }
			symbols "On"

		filter "configurations:Release"
			defines { "NDEBUG" }
			optimize "On"

		-- LTO causes linker to fail with pthread on linux
		filter { "configurations:Release", "toolset:msc" }
			flags {"LinkTimeOptimization"}

		filter "toolset:gcc"
			links{"stdc++fs"}