#include "CustomMultiline.h"

namespace
{
	int InputCallback(ImGuiInputTextCallbackData* data)
	{
		std::string formatted = impl::NewlineFormat(data->Buf, *(float*)data->UserData);
		memcpy_s(data->Buf, 1024, formatted.c_str(), formatted.length() + 1);
		data->BufTextLen = formatted.length();
		data->BufDirty = true;
		return 0;
	}
}

std::string impl::NewlineFormat(const std::string& str, const float width)
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
			continue;
		}
		line += " " + word;
	}
	result += line;
	if ( !str.empty() && str.back() == ' ')
		result.push_back(' ');

	return result;
}

void impl::InlineFormat(std::string& str)
{
	for (char& c : str)
	{
		if (c == '\n')
			c = ' ';
	}
}

bool CustomMultiline(const std::string& tag, std::string& contents)
{
	float widthWindow = ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 2.f;
	float widthText = widthWindow - ImGui::GetStyle().WindowPadding.x * 2.f;
	std::string contentsFormatted = impl::NewlineFormat(contents, widthText);

	char buffer[1024];
	strcpy_s(buffer, contentsFormatted.c_str());
	//strcpy_s(buffer, contents.c_str());

	bool result = ImGui::InputTextMultiline(tag.c_str(), buffer, 1024, ImVec2(widthWindow, -1), ImGuiInputTextFlags_CallbackAlways, InputCallback, &widthText);
	if (result)
	{
		contents = buffer;
		impl::InlineFormat(contents);
	}
	ImVec2 windowSize = ImGui::GetWindowSize();
	windowSize.x -= 1;
	ImGui::SetWindowSize(windowSize);
	windowSize.x += 1;
	ImGui::SetWindowSize(windowSize);
	return result;
}
