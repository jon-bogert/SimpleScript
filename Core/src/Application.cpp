#include "Application.h"

#include "AppData.h"
#include "Debug.h"
#include "WindowManager.h"

#include <filesystem>
#include <fstream>

#include <imgui.h>
#include <yaml-cpp/yaml.h>

void Application::Start()
{
	charactersWindow = WindowManager::NewWindow<CharactersWindow>();
	contentWindow = WindowManager::NewWindow<ContentWindow>();
	inspectorWindow = WindowManager::NewWindow<InspectorWindow>();
	toolbarWindow = WindowManager::NewWindow<ToolbarWindow>();

#ifdef _DEBUG
	script.Load("example.yaml");
#endif //_DEBUG
}

void Application::PreUpdate()
{
}

void Application::OnGUI()
{
	ImGui::BeginMainMenuBar();
	{
		if (ImGui::BeginMenu("File##MM"))
		{
			if (ImGui::MenuItem("New##File"))
			{
				script.New();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Open##File"))
			{
				script.OpenDialogue();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Save##File"))
			{
				script.Save();
			}
			if (ImGui::MenuItem("Save As##File"))
			{
				script.Save(true);
			}
			if (ImGui::MenuItem("Export##File"))
			{
				script.Export();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit##MM"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows##MM"))
		{
			if (ImGui::MenuItem("Characters##Window"))
			{
				charactersWindow->isOpen = true;
			}
			if (ImGui::MenuItem("Content##Window"))
			{
				contentWindow->isOpen = true;
			}
			if (ImGui::MenuItem("Inspector##Window"))
			{
				inspectorWindow->isOpen = true;
			}
			if (ImGui::MenuItem("Toolbar##Window"))
			{
				toolbarWindow->isOpen = true;
			}
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();
	ImGui::DockSpaceOverViewport();
}

void Application::PostUpdate()
{
}

void Application::Destroy()
{
}

void Application::SetWindowHandle(HWND hwnd)
{
	m_hwnd = hwnd;
}

bool Application::LoadSettings()
{
	std::filesystem::path settingsPath = _APPDATA_;
	settingsPath /= L"SimpleScript";
	settingsPath /= L"window.yaml";

	if (!std::filesystem::exists(settingsPath))
	{
		LOG("Could not open %s to load window settings", settingsPath.c_str());
		return false;
	}

	YAML::Node root;

	try
	{
		root = YAML::LoadFile(settingsPath.u8string());
	}
	catch (std::exception)
	{
		LOG("Error while reading window setting file");
		return false;
	}

	if (root["size"].IsDefined() && root["size"].size() >= 2)
	{
		windowWidth = root["size"][0].as<uint32_t>();
		windowHeight = root["size"][1].as<uint32_t>();
	}

	if (root["position"].IsDefined() && root["position"].size() >= 2)
	{
		windowPosX = root["position"][0].as<uint32_t>();
		windowPosY = root["position"][1].as<uint32_t>();
	}

	return true;
}

void Application::SaveSettings()
{
	std::filesystem::path settingsPath = _APPDATA_;
	settingsPath /= L"SimpleScript";

	if (!std::filesystem::exists(settingsPath))
		std::filesystem::create_directories(settingsPath);

	YAML::Node root;

	root["size"].push_back(windowWidth);
	root["size"].push_back(windowHeight);

	root["position"].push_back(windowPosX);
	root["position"].push_back(windowPosY);

	settingsPath /= L"window.yaml";

	std::ofstream file(settingsPath);
	if (!file.is_open())
	{
		LOG("Could not open %s to save window settings", settingsPath.c_str());
		return;
	}

	file << root;
}
