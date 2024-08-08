#include "CharactersWindow.h"

#include "../Application.h"
#include "../Character.h"
#include "../CustomMultiline.h"
#include "../Util.h"

#include <regex>

namespace
{
	struct EditCommand
	{
		enum class Action { Remove, Up, Down };
		size_t index;
		Action action;
	};
}

void CharactersWindow::Start()
{
	name = "Characters";
}

void CharactersWindow::OnGUI()
{
	if (m_selected.empty())
	{
		if (m_editOrder)
		{
			Edit();
			return;
		}
		Search();
		return;
	}
	Info();
}

void CharactersWindow::Search()
{
	if (ImGui::Button("Edit..."))
	{
		m_editOrder = true;
	}
	CharacterManifest& characters = CharacterManifest::Get();
	ImGui::InputText("Search##Characters", m_nameSearchBuffer, 256, ImGuiInputTextFlags_CharsUppercase);
	for (size_t i = 0; i < characters.size(); ++i)
	{
		if (!std::regex_search(characters[i].name, std::regex(m_nameSearchBuffer)))
			continue;
		if (ImGui::MenuItem((characters[i].name + "##SearchChar").c_str()))
		{
			m_selected = characters[i].name;
			break;
		}
	}
	if (strcmp(m_nameSearchBuffer, "") != 0 && !characters.contains(m_nameSearchBuffer))
	{
		ImGui::Separator();
		if (ImGui::Button(("Create New: " + std::string(m_nameSearchBuffer)).c_str()))
		{
			Character newCharacter;
			newCharacter.name = m_nameSearchBuffer;
			newCharacter.color = Utility::RandomColor();
			characters.push_back(newCharacter);
			m_selected = m_nameSearchBuffer;
		}
	}
}

void CharactersWindow::Edit()
{
	if (ImGui::Button("Back"))
	{
		m_editOrder = false;
	}

	ImGui::NewLine();
	CharacterManifest& characters = CharacterManifest::Get();

	EditCommand cmd = { SIZE_MAX, EditCommand::Action::Remove };

	for (size_t i = 0; i < characters.size(); ++i)
	{
		if (ImGui::Button(("-##" + std::to_string(i)).c_str()))
		{
			cmd = { i, EditCommand::Action::Remove };
		}
		ImGui::SameLine();
		if (ImGui::Button(("^##" + std::to_string(i)).c_str()))
		{
			cmd = { i, EditCommand::Action::Up };
		}
		ImGui::SameLine();
		if (ImGui::Button(("v##" + std::to_string(i)).c_str()))
		{
			cmd = { i, EditCommand::Action::Down };
		}
		ImGui::SameLine();
		ImGui::TextColored(ImColor(characters[i].color), " %s", characters[i].name.c_str());
	}

	if (cmd.index == SIZE_MAX)
		return;

	switch (cmd.action)
	{
	case EditCommand::Action::Remove:
		characters.remove(cmd.index);
		break;
	case EditCommand::Action::Up:
		characters.move_up(cmd.index);
		break;
	case EditCommand::Action::Down:
		characters.move_down(cmd.index);
		break;
	}

	cmd.index = SIZE_MAX;
}

void CharactersWindow::Info()
{
	CharacterManifest& manifest = CharacterManifest::Get();
	Character character = manifest[m_selected];

	if (ImGui::Button("Back"))
	{
		m_selected.clear();
		m_editName = false;
		return;
	}

	ImGui::NewLine();
	if (m_editName)
	{
		ImGui::Text("Name:");
		ImGui::InputText(("##name_" + character.name).c_str(), m_nameChangeBuffer, 256, ImGuiInputTextFlags_CharsUppercase);
		if (ImGui::Button("Confirm Change") && !manifest.contains(m_nameChangeBuffer))
		{
			character.name = m_nameChangeBuffer;
			m_editName = false;

			Script& script = Application::Get().script;
			for (size_t i = 0; i < script.NumberOfBlocks(); ++i)
			{
				if (script.GetBlock(i).character == m_selected)
					script.GetBlock(i).character = character.name;
			}

			for (size_t i = 0; i < 256; ++i)
				m_nameChangeBuffer[i] = '\0';
		}
	}
	else
	{
		ImGui::Text("Name:  %s  | ", character.name.c_str());
		ImGui::SameLine();
		if (ImGui::Button(("Edit...##char_" + character.name).c_str()))
		{
			m_editName = true;
			strcpy_s(m_nameChangeBuffer, character.name.length() + 1, character.name.c_str());
		}
	}



	ImGui::NewLine();
	ImGui::ColorEdit3(("Color##char_" + character.name).c_str(), &character.color.r);

	ImGui::NewLine();
	ImGui::Text("Notes:");
	CustomMultiline("##notes_" + character.name, character.notes);

	manifest.update(m_selected, character);

	if (m_selected != character.name)
		m_selected = character.name;
}
