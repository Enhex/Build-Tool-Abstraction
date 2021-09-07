#pragma once

#include <string>

void MakePkg()
{
	namespace bp = boost::process;

	std::string command = "makepkg -si";

	bp::system(command, bp::std_out > stdout);
}