#pragma once
#include <string>

struct TextBlock
{
	enum Type
	{
		Unassigned = -1,
		Slug = 0,
		Action,
		Dialogue,
		Parenthetical
	};

	Type type = Type::Unassigned;
	std::string character = "";
	std::string content;

};

