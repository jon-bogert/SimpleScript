#include "Viewport.h"
#include "Debug.h"

Viewport::Viewport()
{
	m_renderTarget.create(1, 1);
}

void Viewport::Update()
{
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

	ImGui::Begin("Viewport", nullptr, flags);

	ImGuiStyle style = ImGui::GetStyle();
	//ImGui::Begin("Viewport");
	sf::Vector2f size = ImGui::GetWindowSize();
	sf::Vector2f padding = style.FramePadding;
	sf::Vector2f titlebar = { 0.f, ImGui::GetFontSize() + style.FramePadding.y * 2 };
	size -= padding * 4.f;
	size -= titlebar;

	if (m_size != size)
	{
		m_size = size;
		m_renderTarget.create((uint32_t)m_size.x, (uint32_t)m_size.y);

		if (m_resizeCallback != nullptr)
			m_resizeCallback((uint32_t)m_size.x, (uint32_t)m_size.y);
	}

	m_mousePosition = (sf::Vector2f)ImGui::GetMousePos() - (sf::Vector2f)ImGui::GetCursorPos();

	ImGui::Image(m_renderTarget);

	ImGui::End();
}

void Viewport::SetResizeCallback(const std::function<void(uint32_t, uint32_t)>& callback)
{
	m_resizeCallback = callback;
}

sf::RenderTexture& Viewport::GetRenderTarget()
{
	return m_renderTarget;
}
