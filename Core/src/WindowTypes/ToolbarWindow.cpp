#include "ToolbarWindow.h"

#include "Application.h"

#include <regex>

void ToolbarWindow::Start()
{
	name = "Tools";
}

void ToolbarWindow::OnGUI()
{
	ImGui::Text("Add:");
	if (ImGui::Button("Slug Line##add"))
	{
		Add(TextBlock::Slug);
	}
	if (ImGui::Button("Action##add"))
	{
		Add(TextBlock::Action);
	}
	if (ImGui::Button("Parenthetical##add"))
	{
		Add(TextBlock::Parenthetical);
		m_showCharacterAdd = true;
	}
	if (ImGui::Button("Dialogue##add"))
	{
		Add(TextBlock::Dialogue);
		m_showCharacterAdd = true;
	}

	ImGui::NewLine();
	ImGui::Text("Edit:");

	if (ImGui::Button("Move Up##edit"))
	{
		MoveUp();
	}
	if (ImGui::Button("Move Down##edit"))
	{
		MoveDown();
	}
	if (ImGui::Button("Remove##edit"))
	{
		Remove();
	}

	if (m_showCharacterAdd)
	{
		TextBlock& block = Application::Get().script.GetBlock(Application::Get().editIndex);
		xe::Vector2 pos = ImGui::GetWindowSize();
		pos *= 0.5f;
		pos += ImGui::GetWindowPos();

		ImGui::SetNextWindowPos(pos);
		ImGui::Begin("Add Character", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
		
		ImGui::InputText("##SearchAddComp", m_characterAddBuffer, 256, ImGuiInputTextFlags_CharsUppercase);
		CharacterManifest& characters = CharacterManifest::Get();
		for (size_t i = 0; i < characters.size(); ++i)
		{
			if (!std::regex_search(characters[i].name, std::regex(m_characterAddBuffer)))
				continue;
			if (ImGui::MenuItem((characters[i].name + "##editChar").c_str()))
			{
				block.character = characters[i].name;
				m_showCharacterAdd = false;
				break;
			}
		}
		
		ImGui::End();
	}

}

void ToolbarWindow::MoveUp()
{
	Application& app = Application::Get();

	if (app.editIndex == SIZE_MAX)
		return;

	if (app.script.MoveUp(app.editIndex))
		--app.editIndex;
}

void ToolbarWindow::MoveDown()
{
	Application& app = Application::Get();

	if (app.editIndex == SIZE_MAX)
		return;

	if (app.script.MoveDown(app.editIndex))
		++app.editIndex;
}

void ToolbarWindow::Remove()
{
	Application& app = Application::Get();

	if (app.editIndex == SIZE_MAX)
		return;

	app.script.RemoveBlock(app.editIndex);
}

void ToolbarWindow::Add(TextBlock::Type type)
{
	Application& app = Application::Get();
	app.script.AddBlock(type, app.editIndex);

	app.editIndex = (app.editIndex == SIZE_MAX) ? app.script.NumberOfBlocks() - 1 : app.editIndex + 1;
}
