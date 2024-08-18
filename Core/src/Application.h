#pragma once

#include "Script.h"

#include "WindowTypes/CharactersWindow.h"
#include "WindowTypes/ContentWindow.h"
#include "WindowTypes/InspectorWindow.h"
#include "WindowTypes/ToolbarWindow.h"

#include <windows.h>
#include <filesystem>
#include <cstdint>

#ifndef VERSION_MAJOR
#define VERSION_MAJOR 0
#endif
#ifndef VERSION_MINOR
#define VERSION_MINOR 2
#endif
#ifndef VERSION_PATCH
#define VERSION_PATCH 3
#endif

enum class UIStyle { Dark, Light, Custom};

class Application final
{
	Application() {}

public:
	static Application& Get() { static Application instance; return instance; }
	~Application() = default;
	Application(const Application& other) = delete;
	Application(const Application&& other) = delete;
	Application operator=(const Application& other) = delete;
	Application operator=(const Application&& other) = delete;
	const wchar_t* name = L"SimpleScript";
	uint32_t windowWidth = 1280;
	uint32_t windowHeight = 720;
	uint32_t windowPosX = 100;
	uint32_t windowPosY = 100;
	UIStyle style = UIStyle::Custom;
	bool useFloatingWindows = true;

	void Start();
	void PreUpdate();
	void OnGUI();
	void PostUpdate();
	void Destroy();
	void SetWindowHandle(HWND hwnd);
	bool LoadSettings();
	void SaveSettings();

	void SaveWindow();
	void TryQuit();

	bool isRunning = true;
	bool isSaved = true;
	bool showSaveWindow = false;

	ContentWindow* contentWindow = nullptr;
	CharactersWindow* charactersWindow = nullptr;
	ToolbarWindow* toolbarWindow = nullptr;
	InspectorWindow* inspectorWindow = nullptr;

	Script script;
	size_t editIndex = SIZE_MAX;

	std::vector<std::filesystem::path> recentFiles;
	const int RECENT_MAX = 10;
	std::filesystem::path lastOpen;
	std::filesystem::path lastSave;
	std::filesystem::path lastExport;

private:
	HWND m_hwnd;

};

