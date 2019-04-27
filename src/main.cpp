#include "MSBuild.h"
#include "Make.h"

namespace fs = boost::filesystem;

// return true if successfully found the build tool
bool find_and_use_tool(fs::path const& path)
{
	for (auto const& entry : fs::directory_iterator(path))
	{
		// check for Visual Studio solution file
		if (entry.path().extension() == ".sln") {
			return MSBuild(entry.path());
		}
		// check for Makefile
		else if (entry.path().filename() == "Makefile") {
			Make();
			return true;
		}
	}

	return false;
}

bool find_and_use_tool()
{
	return find_and_use_tool(fs::current_path());
}

int main(int argc, char* argv[])
{
	if (argc > 2) {
		std::cerr << "Too many arguments.\n";
		return EXIT_FAILURE;
	}

	bool success;

	if (argc == 1) {
		success = find_and_use_tool();
	}
	else {
		success = find_and_use_tool(argv[1]);
	}

	if (!success) {
		std::cerr << "could not find build tool file.\n";
		return EXIT_FAILURE;
	}
}