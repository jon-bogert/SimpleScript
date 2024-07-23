#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <memory>
#include <functional>

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

	void SetResizeCallback(const std::function<void(uint32_t, uint32_t)>& resizeCallback);

private:
	std::unique_ptr<sf::RenderWindow> m_window = nullptr;
	std::function<void(uint32_t, uint32_t)> m_resizeCallback = nullptr;

	sf::Clock m_deltaTimer;
};

