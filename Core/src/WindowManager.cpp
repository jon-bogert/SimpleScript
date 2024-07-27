#include "WindowManager.h"

#include "AppData.h"
#include "Application.h"
#include "Debug.h"
#include "Editor/Editor.h"

#include <imgui-SFML.h>
#include <imgui.h>
#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <fstream>

void WindowManager::Start(uint32_t width, uint32_t height)
{
	if (m_window != nullptr)
	{
		ImGui::SFML::Shutdown();
		m_window = nullptr;
	}

	sf::Vector2u size;

	if (LoadSettings(size))
	{
		m_window = std::make_unique<sf::RenderWindow>(
			sf::VideoMode(size.x, size.y),
			"SimpleScript",
			sf::Style::Default);

		//m_window->getSystemHandle()

		m_windowSize = size;
	}
	else
	{
		m_window = std::make_unique<sf::RenderWindow>(
			sf::VideoMode(width, height),
			"SimpleScript",
			sf::Style::Default);
		m_windowSize = { width, height };
	}

	m_deltaTimer.restart();

	ImGui::SFML::Init(*m_window);

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void WindowManager::Update()
{
	sf::Event event;
	while (m_window->pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(*m_window, event);

		if (event.type == sf::Event::Closed)
		{
			m_window->close();
		}
		if (event.type == sf::Event::Resized && m_resizeCallback != nullptr)
		{
			uint32_t width = event.size.width;
			uint32_t height = event.size.height;
			m_windowSize = { width, height };
			m_resizeCallback(width, height);
		}
		if (event.type == sf::Event::MouseWheelScrolled)
		{
			//Application::Get().UpdateScroll(-event.mouseWheelScroll.delta * 2.f);
			if (m_scrollCallback != nullptr)
				m_scrollCallback(event.mouseWheelScroll.delta);
		}
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left &&
				m_leftClickCallback != nullptr)
				m_leftClickCallback();
		}
	}

	ImGui::SFML::Update(*m_window, m_deltaTimer.restart());

	if (m_editor != nullptr)
		m_editor->Update();

	ImGui::Begin("Hello, world!", nullptr, ImGuiWindowFlags_DockNodeHost);
	ImGui::Button("Look at this pretty button");
	ImGui::End();

	m_editor->GetViewport().GetRenderTarget().clear(sf::Color::Black);
	if (Application::Get().GetActiveProject() != nullptr)
	{
		Application::Get().GetActiveProject()->RenderTo(&m_editor->GetViewport().GetRenderTarget());
		Application::Get().GetEditor()->GetTextEditor().RenderTo(&m_editor->GetViewport().GetRenderTarget());
	}
	m_editor->GetViewport().GetRenderTarget().display();

	m_window->clear();
	ImGui::SFML::Render(*m_window);
	m_window->display();
}

void WindowManager::Shutdown()
{
	if (m_window == nullptr)
		return;

	SaveSettings();

	ImGui::SFML::Shutdown();
	m_window = nullptr;
}

bool WindowManager::IsOpen() const
{
	if (m_window == nullptr)
		return false;

	return m_window->isOpen();
}

void WindowManager::Close()
{
	if (m_window == nullptr)
		return;

	m_window->close();
}

void WindowManager::SetEditor(Editor* editor)
{
	m_editor = editor;
}

void WindowManager::SetResizeCallback(const std::function<void(uint32_t, uint32_t)>& resizeCallback)
{
	m_resizeCallback = resizeCallback;
}

void WindowManager::SetScrollCallback(const std::function<void(float)>& scrollCallback)
{
	m_scrollCallback = scrollCallback;
}

void WindowManager::SetLeftClickCallback(const std::function<void(void)> leftClickCallback)
{
	m_leftClickCallback = leftClickCallback;
}

void WindowManager::SaveSettings()
{
	sf::Vector2u size = m_windowSize;

	std::filesystem::path settingsPath = _APPDATA_;
	settingsPath /= L"SimpleScript";

	if (!std::filesystem::exists(settingsPath))
		std::filesystem::create_directories(settingsPath);

	YAML::Node root;

	root["size"].push_back(size.x);
	root["size"].push_back(size.y);

	settingsPath /= L"window.yaml";

	std::ofstream file(settingsPath);
	if (!file.is_open())
	{
		LOG("Could not open %s to save window settings", settingsPath.c_str());
		return;
	}

	file << root;
}

bool WindowManager::LoadSettings(sf::Vector2u& out_size)
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
		out_size.x = root["size"][0].as<uint32_t>();
		out_size.y = root["size"][1].as<uint32_t>();
	}

	return true;
}
