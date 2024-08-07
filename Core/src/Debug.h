#pragma once

#ifdef _DEBUG

#include <imgui.h>

#include <cstdio>
#include <string>
#include <vector>

class DebugWindow
{
	DebugWindow()
	{
		m_messages.reserve(m_msgLen);
	}

public:
	static DebugWindow& Get() { static DebugWindow instance; return instance; }
	~DebugWindow() = default;
	DebugWindow(const DebugWindow& other) = delete;
	DebugWindow(const DebugWindow&& other) = delete;
	DebugWindow operator=(const DebugWindow& other) = delete;
	DebugWindow operator=(const DebugWindow&& other) = delete;

	void AddMsg(const std::string& msg)
	{
		if (m_messages.size() >= m_msgLen)
		{
			m_messages.erase(m_messages.begin());
		}
		m_messages.push_back(msg);
	}

	void OnGUI()
	{
		ImGui::Begin("Debug");
		for (const std::string& msg : m_messages)
		{
			ImGui::Text(msg.c_str());
		}
		ImGui::End();
	}

private:
	std::vector<std::string> m_messages;
	size_t m_msgLen = 100;
};

#define LOG(format, ...)\
	do{\
		char buffer[256];\
		int res = snprintf(buffer, std::size(buffer), ##format##"\n", __VA_ARGS__);\
		DebugWindow::Get().AddMsg(buffer);\
	}while(false)

#else
#define LOG(format, ...)
#endif