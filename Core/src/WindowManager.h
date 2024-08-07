#pragma once

#include <imgui.h>

#include <memory>
#include <string>
#include <vector>

class Window
{
public:
	Window() = default;
	virtual ~Window() = default;

	bool isOpen = true;
	std::string name = "Window##";
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;

	virtual void Start() {}
	virtual void PreUpdate() {}
	virtual void Specs() {}			//To be called before ImGui::Begin()
	virtual void OnGUI() = 0;		//'OnGUI' Must be overridden
	virtual void PostUpdate() {}
	virtual void Destroy() {}
};

class WindowManager final
{
	WindowManager() {}
	static WindowManager& Get();
	std::vector<std::unique_ptr<Window>> m_windows;

public:
	template <class WinType>
	static WinType* NewWindow();

	static void PreUpdate();
	static void OnGUI();
	static void PostUpdate();
	static void Destroy();
};

template <class WinType>
inline WinType* WindowManager::NewWindow()
{
	static_assert(std::is_base_of_v<Window, WinType>, "WindowManager: Cannot add Non-Window type");
	Get().m_windows.push_back(std::make_unique<WinType>());
	Get().m_windows.back()->Start();
	return (WinType*)Get().m_windows.back().get();
}