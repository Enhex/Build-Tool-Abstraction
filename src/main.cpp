#include "MSBuild.h"
#include "Make.h"

// return true if successfully found the build tool
bool find_and_use_tool(fs::path const& path)
{
	auto check_file = [](fs::path const& filepath){
		// check for Visual Studio solution file
		if (filepath.extension() == ".sln") {
			//vs_upgrade(filepath); //TODO should be optional
			return MSBuild(filepath);
		}
		// check for Makefile
		else if (filepath.filename() == "Makefile") {
			Make();
			return true;
		}

		return false;
	};

	if (fs::is_directory(path))
	{
		for (auto const& entry : fs::directory_iterator(path))
		{
			if(check_file(entry.path()))
				return true;
		}
	}
	else {
		if(check_file(path))
			return true;
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