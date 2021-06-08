#pragma once

#include <thread>
#include <string>

void Make()
{
	namespace bp = boost::process;

	std::string command = "make -j";
	command += std::to_string(std::thread::hardware_concurrency());

	bp::system(command, bp::std_out > stdout);
}