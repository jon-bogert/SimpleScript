#pragma once

#include <imgui.h>

#include <cstring>
#include <sstream>
#include <string>

namespace impl
{
	std::string NewlineFormat(const std::string& str, const float width);
	void InlineFormat(std::string& str);
}

bool CustomMultiline(const std::string& tag, std::string& contents);