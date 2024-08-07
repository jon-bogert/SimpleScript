#pragma once

#include <imgui.h>

#include <sstream>
#include <string>

#ifndef CUST_ML_BUFFER_SIZE
#define CUST_ML_BUFFER_SIZE 1024
#endif //!CUST_ML_BUFFER_SIZE

#ifndef CUST_ML_LINE_HEIGHT
#define CUST_ML_LINE_HEIGHT 5
#endif //!CUST_ML_LINE_HEIGHT

namespace impl
{
	std::string NewlineFormat(const std::string& str, const float width)
	{
		std::stringstream stream(str);
		std::string result;
		std::string line;
		std::string word;

		while (std::getline(stream, word, ' '))
		{
			if (line.empty())
			{
				line += word;
				continue;
			}
			if (ImGui::CalcTextSize((line + " " + word).c_str()).x > width)
			{
				result += line + "\n";
				line = word;
			}
			line += " " + word;
		}
		result += line;

		return result;
	}

	void InlineFormat(std::string& str)
	{
		for (char& c : str)
		{
			if (c == '\n')
			{
				c = ' ';
			}
		}
	}

	int CustomMultilineCallback(ImGuiInputTextCallbackData* data)
	{
		char* buffer = data->Buf;
		float width = *(float*)data->UserData;
		if (ImGui::CalcTextSize(buffer).x > width)
		{
			std::string newBuff = buffer;
			InlineFormat(newBuff);
			newBuff = NewlineFormat(newBuff, width);
			memcpy(buffer, newBuff.c_str(), newBuff.length() + 1);
		}

		return data->BufTextLen;
	}
}

bool CustomMultiline(const std::string& tag, std::string& content)
{
	ImGuiStyle& style = ImGui::GetStyle();
	float width = ImGui::GetWindowWidth() - style.WindowPadding.x * 4.f;

	ImGuiInputTextFlags flags = ImGuiInputTextFlags_CallbackEdit 
		| ImGuiInputTextFlags_CtrlEnterForNewLine;

	content = impl::NewlineFormat(content, width);

	char buffer[CUST_ML_BUFFER_SIZE];
	memcpy_s(buffer, CUST_ML_BUFFER_SIZE, content.c_str(), content.length() + 1);


	bool result = ImGui::InputTextMultiline(tag.c_str(), buffer, CUST_ML_BUFFER_SIZE,
		ImVec2(width, ImGui::GetTextLineHeightWithSpacing() * CUST_ML_LINE_HEIGHT), flags,
		impl::CustomMultilineCallback, &width);

	content = buffer;
	impl::InlineFormat(content);

	return result;
}