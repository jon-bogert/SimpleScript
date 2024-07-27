#pragma once

#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>

#include <functional>

class Viewport
{
public:
	Viewport();

	void Update();
	void SetResizeCallback(const std::function<void(uint32_t, uint32_t)>& callback);
	sf::RenderTexture& GetRenderTarget();
	sf::Vector2f GetMousePosition() const { return m_mousePosition; }

private:
	sf::Vector2f m_size = { 0.f, 0.f };

	sf::Vector2f m_mousePosition;

	sf::RenderTexture m_renderTarget;
	std::function<void(uint32_t, uint32_t)> m_resizeCallback = nullptr;
};