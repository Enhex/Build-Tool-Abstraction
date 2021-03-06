#pragma once

// Workaround for a boost/mingw bug.
// This must occur before the inclusion of the boost/process.hpp header.
// Taken from https://github.com/boostorg/process/issues/96
#ifndef __kernel_entry
    #define __kernel_entry
#endif

#include <boost/process.hpp>
#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

// can use either project or solution file
bool MSBuild(fs::path const& file)
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
	auto command = vswhere_path + R"( -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe)";

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

	// call MSBuild
	command.clear();
	command += '\"';
	command += msbuild_path;
	command += "\" \"" + file.string() + '\"';

	try {
		bp::system(command, bp::std_out > stdout);
	}
	catch (std::exception & e) {
		std::cerr << e.what() << std::endl;
		return false;
	}

	return true;
}

bool vs_upgrade(fs::path const& file)
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


	// find devenv
	auto command = vswhere_path + R"( -legacy -latest -property installationPath)";

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

	const auto devenv_path = run_command() + R"(\Common7\IDE\devenv.exe)";

	// call devenv
	command.clear();
	command += '\"';
	command += devenv_path;
	command += "\" \"" + file.string() + "\" /Upgrade";

	try {
		bp::system(command, bp::std_out > stdout);
	}
	catch (std::exception & e) {
		std::cerr << e.what() << std::endl;
		return false;
	}

	return true;
}