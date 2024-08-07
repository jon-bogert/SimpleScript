#include "Structs.h"
#include "Util.h"

#include <sstream>

#define DIALOGUE_LIMIT 36
#define ACTION_LIMIT 57
#define COLOR_CLIP 0.1f

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

	xe::Color ClipColor(const xe::Color& color)
	{
		xe::Color result = color;
		float highest = std::max(result.r, std::max(result.g, result.b));
		float factor = (COLOR_CLIP / highest);
		result.r *= factor;
		result.g *= factor;
		result.b *= factor;
		return result;
	}
	xe::Color RandomColor()
	{
		xe::Color8 color;
		bool isLightEnough = false;
		while (!isLightEnough)
		{
			color.r = xe::Math::Random::Range(0, 256);
			color.g = xe::Math::Random::Range(0, 256);
			color.b = xe::Math::Random::Range(0, 256);
			uint8_t lightest = std::max(color.r, std::max(color.g, color.b));
			isLightEnough = (lightest >= 200);
		}
		return color;
	}
}