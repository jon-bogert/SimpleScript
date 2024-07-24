#include "Editor.h"

void Editor::Update()
{
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File##MainMenu"))
	{
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Edit##MainMenu"))
	{
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
	ImGui::DockSpaceOverViewport();

	m_viewport.Update();
}

Viewport& Editor::GetViewport()
{
	return m_viewport;
}
