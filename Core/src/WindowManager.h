#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <memory>
#include <functional>

class Editor;

class WindowManager
{
public:
	WindowManager() = default;
	~WindowManager() { Shutdown(); }

	void Start(uint32_t width, uint32_t height);
	void Update();
	void Shutdown();

	bool IsOpen() const;
	void Close();

	void SetEditor(Editor* editor);
	void SetResizeCallback(const std::function<void(uint32_t, uint32_t)>& resizeCallback);
	void SetScrollCallback(const std::function<void(float)>& scrollCallback);
	void SetLeftClickCallback(const std::function<void(void)> leftClickCallback);

private:
	void SaveSettings();
	bool LoadSettings(sf::Vector2u& out_size);

	std::unique_ptr<sf::RenderWindow> m_window = nullptr;

	std::function<void(uint32_t, uint32_t)> m_resizeCallback = nullptr;
	std::function<void(float)> m_scrollCallback = nullptr;
	std::function<void(void)> m_leftClickCallback = nullptr;

	sf::Vector2u m_windowSize;

	Editor* m_editor = nullptr;

	sf::Clock m_deltaTimer;
};

