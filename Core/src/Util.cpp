#include "Util.h"

#include <sstream>

#define DIALOGUE_LIMIT 36
#define ACTION_LIMIT 57

namespace Utility
{
	void AllCaps(std::string& str)
	{
		for (char& c : str)
		{
			if (c >= 'a' && c <= 'z')
				c -= 32;
		}
	}

	std::wstring TwoDigUInt(uint32_t i)
	{
		std::wstring str;
		if (i < 10)
			str += L"0";

		str += std::to_wstring(i);
		return str;
	}

	std::vector<std::string> DialogueLineBreaks(const std::string& line)
	{
		std::stringstream stream(line);
		std::vector<std::string> result;

		int counter = 0;
		std::string word;
		std::string currLine;

		while (std::getline(stream, word, ' '))
		{
			counter += word.length();
			if (counter + 1 > DIALOGUE_LIMIT)
			{
				result.push_back(currLine);
				counter = word.length();
				currLine = word;
				continue;
			}
			if (!currLine.empty())
			{
				currLine.push_back(' ');
				++counter;
			}
			currLine.append(word);
		}
		result.push_back(currLine);

		return result;
	}

	std::vector<std::string> ActionLineBreaks(const std::string& line)
	{
		std::stringstream stream(line);
		std::vector<std::string> result;

		int counter = 0;
		std::string word;
		std::string currLine;

		while (std::getline(stream, word, ' '))
		{
			counter += word.length();
			if (counter + 1 > ACTION_LIMIT)
			{
				result.push_back(currLine);
				counter = word.length();
				currLine = word;
				continue;
			}
			if (!currLine.empty())
			{
				currLine.push_back(' ');
				++counter;
			}
			currLine.append(word);
		}
		result.push_back(currLine);

		return result;
	}

	std::string SlugFormat(const uint32_t number, const std::string& line)
	{
		std::string numstr = std::to_string(number);
		std::string result = numstr;

		result.push_back('\t');
		result.append(line);
		for (int i = 0; i < ACTION_LIMIT - line.length() - numstr.length(); ++i)
		{
			result.push_back(' ');
		}
		result.append(numstr);

		return result;
	}
}