#include "ContentWindow.h"

#include "../Application.h"
#include "../CustomMultiline.h"
#include "../Debug.h"
#include "../Util.h"

std::string str;

void ContentWindow::Start()
{
	name = "Content";
}

void ContentWindow::OnGUI()
{
	Application& app = Application::Get();
	Script& script = app.script;

	if (script.NumberOfBlocks() == 0)
	{
		ImGui::Text("No content yet...");
		return;
	}

	bool isHeaderOpen = false;
	uint32_t slugCount = 0;
	std::string lastCharacter = "";
	bool wasLastDialogue = false; // or parethetical

	for (size_t i = 0; i < script.NumberOfBlocks(); ++i)
	{
		TextBlock& block = script.GetBlock(i);

		if (block.type == TextBlock::Slug)
		{
			if (isHeaderOpen)
				ImGui::NewLine();

			std::string content = (block.content.empty()) ? "<EMPTY>" : block.content;
			isHeaderOpen = ImGui::CollapsingHeader(("[" + std::to_string(slugCount++) + "] " + content).c_str());
			if (ImGui::IsItemClicked(1))
			{
				app.editIndex = i;
			}
			continue;
		}

		if (!isHeaderOpen)
			continue;

		if (block.type == TextBlock::Dialogue
			|| block.type == TextBlock::Parenthetical)
		{
			if (!wasLastDialogue || lastCharacter != block.character)
			{
				xe::Color color = (CharacterManifest::Get().contains(block.character)) ?
					CharacterManifest::Get()[block.character].color :
					xe::Color::White;
				xe::Color bgColor = color;

				float highest = max(color.r, max(color.g, color.b));
				float factor = 0.1f / highest;

				bgColor *= factor;
				bgColor.a = 1.f;

				ImGui::NewLine();
				ImGui::BeginTable("##char", 1);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImColor((ImVec4)bgColor));
				ImGui::TextColored(color, (" " + block.character).c_str());

				ImGui::EndTable();

				lastCharacter = block.character;
			}
			wasLastDialogue = true;

			if (i == app.editIndex)
			{
				ImGui::BeginTable("##char", 1);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImColor(ImVec4(0.4, 0.4, 0.4, 1.f)));
			}

			std::string content{};
			if (block.type == TextBlock::Parenthetical)
				content.append("\t(");

			content.append(block.content.empty() ? " <empty> " : block.content);

			if (block.type == TextBlock::Parenthetical)
				content.push_back(')');

			ImGui::TextWrapped(content.c_str());
			if (i == app.editIndex)
			{
				ImGui::EndTable();
			}

			if (ImGui::IsItemClicked())
			{
				app.editIndex = i;
			}

			continue;
		}
		wasLastDialogue = false;

		ImGui::NewLine();
		ImGui::BeginTable("##char", 1);

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		float colVal = (i == app.editIndex) ? 0.2f : 0.1f;
		ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImColor(ImVec4(colVal, colVal, colVal, 1.f)));
		std::string content = (block.content.empty()) ? " <empty> " : block.content;
		ImGui::TextColored(ImVec4(0.6, 0.6, 0.6, 1.f), impl::NewlineFormat(content, ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 4.f).c_str());

		if (ImGui::IsItemClicked())
		{
			app.editIndex = i;
		}

		ImGui::EndTable();
	}
}
