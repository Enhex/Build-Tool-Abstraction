#include "MSBuild.h"
#include "Make.h"

// return true if successfully found the build tool
bool find_and_use_tool()
{
	namespace fs = boost::filesystem;

	auto const current_path = fs::current_path();

	for (auto const& entry : fs::directory_iterator(current_path))
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

int main()
{
	if (!find_and_use_tool()) {
		std::cerr << "could not find build tool file.\n";
		return 1;
	}
}