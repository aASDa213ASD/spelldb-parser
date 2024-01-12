#pragma once

#include <string>

std::string add_quotes(std::string str)
{
	return ('"' + str + '"');
}

std::string to_lower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), (int(*)(int)) std::tolower);
	return str;
}