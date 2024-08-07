#include "InspectorWindow.h"

#include "../Application.h"

#include <XephInput/InputSystem.h>

#include <regex>

namespace
{
	const char* k_typeCombo = "Slug Line\0Action\0Parethetical\0Dialogue";
}

void InspectorWindow::Start()
{
	name = "Inspector";
}

void InspectorWindow::OnGUI()
{
	Application& app = Application::Get();
	if (app.editIndex == SIZE_MAX)
	{
		ImGui::Text("No element selected...");
		return;
	}

	TextBlock& block = app.script.GetBlock(app.editIndex);

	if (block.type == TextBlock::Unassigned)
	{
		ImGui::Text("ERROR -- Type is unassigned...");
	}
	int typeInt = (int)block.type;

	ImGui::Text("Type:");
	if (ImGui::Combo("##Typeinsp", &typeInt, k_typeCombo))
	{
		block.type = (TextBlock::Type)typeInt;
	}

	if (block.type == TextBlock::Dialogue
		|| block.type == TextBlock::Parenthetical)
	{
		ImGui::NewLine();
		ImGui::Text("Character:  %s  | ", block.character.c_str());
		ImGui::SameLine();

		if (ImGui::Button("Edit..."))
		{
			m_editCharacter = true;
		}

		if (m_editCharacter)
		{
			ImVec2 nextPos = (xe::Vector2)ImGui::GetCursorPos() + ImGui::GetWindowPos();
			ImGui::SetNextWindowPos(nextPos);
			ImGui::Begin("Edit Character##Insp", nullptr,
				ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoDecoration |
				ImGuiWindowFlags_NoMove);

			if (xe::InputSystem::GetMouseDown(xe::Mouse::Button::Left) && !ImGui::IsWindowHovered())
				m_editCharacter = false;

			ImGui::InputText("##SearchAddComp", m_editCharacterBuffer, 256, ImGuiInputTextFlags_CharsUppercase);
			CharacterManifest& characters = CharacterManifest::Get();
			for (size_t i = 0; i < characters.size(); ++i)
			{
				if (!std::regex_search(characters[i].name, std::regex(m_editCharacterBuffer)))
					continue;
				if (ImGui::MenuItem((characters[i].name + "##editChar").c_str()))
				{
					block.character = characters[i].name;
					m_editCharacter = false;
					break;
				}
			}

			ImGui::End();
		}

		char buffer[1024];
		ImGui::NewLine();
		ImGui::Text("Content:");
		memcpy_s(buffer, 1024, block.content.c_str(), block.content.length() + 1);
		std::string tag = "##cont_" + std::to_string(app.editIndex);
		if (ImGui::InputTextMultiline(tag.c_str(), buffer, 1024, ImVec2(-1, -1)))
		{
			block.content = buffer;
		}
	}
	else if (block.type == TextBlock::Action)
	{
		char buffer[1024];
		ImGui::NewLine();
		ImGui::Text("Content:");
		memcpy_s(buffer, 1024, block.content.c_str(), block.content.length() + 1);
		std::string tag = "##cont_" + std::to_string(app.editIndex);
		if (ImGui::InputTextMultiline(tag.c_str(), buffer, 1024, ImVec2(-1, -1)))
		{
			block.content = buffer;
		}
	}
	else if (block.type == TextBlock::Slug)
	{
		char buffer[1024];
		ImGui::NewLine();
		ImGui::Text("Content:");
		memcpy_s(buffer, 1024, block.content.c_str(), block.content.length() + 1);
		std::string tag = "##cont_" + std::to_string(app.editIndex);
		if (ImGui::InputText(tag.c_str(), buffer, 1024, ImGuiInputTextFlags_CharsUppercase))
		{
			block.content = buffer;
		}
	}
}
