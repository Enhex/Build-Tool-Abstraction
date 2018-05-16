#pragma once

void Make()
{
	namespace bp = boost::process;
	bp::system("make", bp::std_out > stdout);
}