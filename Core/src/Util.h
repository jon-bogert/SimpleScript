#pragma once

#include <string>
#include <vector>

namespace Utility
{
	void AllCaps(std::string& str);
	std::wstring TwoDigUInt(uint32_t i);

	std::vector<std::string> DialogueLineBreaks(const std::string& line);
	std::vector<std::string> ActionLineBreaks(const std::string& line);
	std::string SlugFormat(const uint32_t number, const std::string& line);
}

