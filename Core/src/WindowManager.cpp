#include "WindowManager.h"

#include <imgui-SFML.h>
#include <imgui.h>

void WindowManager::Start(uint32_t width, uint32_t height)
{
	if (m_window != nullptr)
	{
		ImGui::SFML::Shutdown();
		m_window = nullptr;
	}

	m_window = std::make_unique<sf::RenderWindow>(
		sf::VideoMode(width, height),
		"SimpleScript",
		sf::Style::Default);

	m_deltaTimer.restart();

	ImGui::SFML::Init(*m_window);
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
			m_resizeCallback(width, height);
		}
	}

	ImGui::SFML::Update(*m_window, m_deltaTimer.restart());

	ImGui::Begin("Hello, world!");
	ImGui::Button("Look at this pretty button");
	ImGui::End();

	m_window->clear();
	ImGui::SFML::Render(*m_window);
	m_window->display();
}

void WindowManager::Shutdown()
{
	if (m_window == nullptr)
		return;

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

void WindowManager::SetResizeCallback(const std::function<void(uint32_t, uint32_t)>& resizeCallback)
{
	m_resizeCallback = resizeCallback;
}
