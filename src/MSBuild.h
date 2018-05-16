#pragma once

#include <boost/process.hpp>
#include <iostream>
#include <string>
//#include <filesystem> //TODO still not supported well enough
#include <boost/filesystem.hpp>

// can use either project or solution file
bool MSBuild(boost::filesystem::path const& file)
{
	namespace bp = boost::process;

	// find vswhere
	std::string program_files;
	if (const char* env_p = std::getenv("ProgramFiles(x86)")) {
		program_files = env_p;
	}
	else {
		return false;
	}

	auto const vswhere_path = '\"' + program_files + R"(\Microsoft Visual Studio\Installer\vswhere.exe")";


	// find MSBuild
	auto command = vswhere_path + " -latest -products * -requires Microsoft.Component.MSBuild -property installationPath";

	// run command and return return single line
	auto run_command = [&command]()
	{
		std::string output;
		bp::ipstream is;
		bp::system(command, bp::std_out > is);
		std::getline(is, output);
		if (output.back() == '\r')
			output.pop_back();
		return output;
	};

	const auto msbuild_path = run_command();

	// find VS version
	command.clear();
	command += vswhere_path + " -latest -products * -requires Microsoft.Component.MSBuild -property catalog_productDisplayVersion";

	auto const vs_version = run_command();
	auto const vs_version_major = vs_version.substr(0, vs_version.find_first_of('.'));

	// call MSBuild
	command.clear();
	command += '\"';
	command += msbuild_path;
	command += R"(\MSBuild\)" + vs_version_major + R"(.0\Bin\MSBuild.exe" ")" + file.string() + '\"';

	bp::system(command, bp::std_out > stdout);

	return true;
}