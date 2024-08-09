#include "Application.h"

#include "AppData.h"
#include "Debug.h"
#include "WindowManager.h"

#include <fstream>

#include <imgui.h>
#include <yaml-cpp/yaml.h>
#include <XephInput/InputSystem.h>

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
	if (xe::InputSystem::GetKeyHold(xe::Key::Ctrl))
	{
		if (xe::InputSystem::GetKeyHold(xe::Key::Shift))
		{
			if (xe::InputSystem::GetKeyDown(xe::Key::S))
			{
				script.Save(true);
			}
		}
		else
		{
			if (xe::InputSystem::GetKeyDown(xe::Key::N))
			{
				script.New();
			}
			if (xe::InputSystem::GetKeyDown(xe::Key::O))
			{
				script.OpenDialogue();
			}
			if (xe::InputSystem::GetKeyDown(xe::Key::S))
			{
				script.Save();
			}
			if (xe::InputSystem::GetKeyDown(xe::Key::E))
			{
				script.Export();
			}
		}
	}
}

void Application::OnGUI()
{
	ImGui::BeginMainMenuBar();
	{
		if (ImGui::BeginMenu("File##MM"))
		{
			if (ImGui::MenuItem("New##File", "Ctrl+N"))
			{
				script.New();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Open##File", "Ctrl+O"))
			{
				script.OpenDialogue();
			}
			if (ImGui::BeginMenu("Open Recent##File"))
			{
				if (recentFiles.empty())
				{
					ImGui::TextDisabled("No recent projects");
				}
				else
				{
					for (const std::filesystem::path& file : recentFiles)
					{
						if (ImGui::MenuItem(file.filename().u8string().c_str()))
						{
							script.Load(file);
						}
					}
				}
				ImGui::EndMenu();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Save##File", "Ctrl+S"))
			{
				script.Save();
			}
			if (ImGui::MenuItem("Save As##File", "Ctrl+Shift+S"))
			{
				script.Save(true);
			}
			if (ImGui::MenuItem("Export##File", "Ctrl+E"))
			{
				script.Export();
			}
			ImGui::EndMenu();
		}
		//if (ImGui::BeginMenu("Edit##MM"))
		//{
		//	ImGui::EndMenu();
		//}
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
	settingsPath /= L"settings.yaml";

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

	if (root["recent"].IsDefined())
	{
		for (const YAML::Node& recent : root["recent"])
		{
			std::filesystem::path path(recent.as<std::string>());
			if (!std::filesystem::exists(path))
				continue;

			recentFiles.push_back(path);
		}
	}

	if (root["last-open"].IsDefined())
	{
		lastOpen = root["last-open"].as<std::string>();
	}
	if (root["last-save"].IsDefined())
	{
		lastSave = root["last-save"].as<std::string>();
	}
	if (root["last-export"].IsDefined())
	{
		lastExport = root["last-export"].as<std::string>();
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

	if (!recentFiles.empty())
	{
		for (const std::filesystem::path& path : recentFiles)
		{
			root["recent"].push_back(path.u8string());
		}
	}

	root["last-open"] = lastOpen.u8string();
	root["last-save"] = lastSave.u8string();
	root["last-export"] = lastExport.u8string();

	settingsPath /= L"settings.yaml";

	std::ofstream file(settingsPath);
	if (!file.is_open())
	{
		LOG("Could not open %s to save window settings", settingsPath.c_str());
		return;
	}

	file << root;
}

void Application::SaveWindow()
{
	xe::Vector2 pos = { (float)windowWidth, (float)windowHeight };
	pos *= 0.5;
	pos.x += windowPosX;
	pos.y += windowPosY;
	ImGui::SetNextWindowPos(pos, 0, {0.5f, 0.5f});
	ImGui::Begin("Save Before Close", nullptr,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoMove);
	ImGui::TextColored(xe::Color::Yellow, "Warning");
	ImGui::NewLine();
	ImGui::Text("You have unsaved changes.\nWould you like to save before closing?");
	ImGui::NewLine();

	if (ImGui::Button("Save##savewin"))
	{
		script.Save();
		isRunning = false;
	}
	ImGui::SameLine();
	if (ImGui::Button(" Don't Save##savewin"))
	{
		isRunning = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel##savewin"))
	{
		showSaveWindow = false;
	}

	ImGui::End();
}

void Application::TryQuit()
{
	if (!isSaved)
	{
		showSaveWindow = true;
		return;
	}
	isRunning = false;
}
