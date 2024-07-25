#include "Project.h"

#include "Application.h"
#include "Debug.h"
#include "GUI/CharacterBlock.h"
#include "Script.h"

void Project::Start()
{
	Application::Get().GetEditor()->GetViewport().SetResizeCallback([&](uint32_t w, uint32_t h) { Resize(w, h); });
}

void Project::Load(const std::filesystem::path& filepath)
{
	LOG("Start Load");
	Script* script = new Script();
	script->Load(filepath);

	CharacterManifest& characters = CharacterManifest::Get();

	m_name = script->GetName();

	CharacterBlock* lastCharacter = nullptr;
	bool wasLastDialogue = false;

	for (size_t i = 0; i < script->NumberOfBlocks(); ++i)
	{
		TextBlock& block = script->GetBlock(i);
		LOG("Script Item: %i, character?: %s", i, block.character.c_str());

		if (block.type == TextBlock::Dialogue || block.type == TextBlock::Parenthetical)
		{
			if (lastCharacter == nullptr
				|| (wasLastDialogue && block.character != lastCharacter->GetCharacter()))
			{
				if (lastCharacter != nullptr)
					lastCharacter->SetRecalculateVisuals(true);

				lastCharacter = (CharacterBlock*)m_blocks.emplace_back(std::make_unique<CharacterBlock>()).get();
				lastCharacter->Start(Application::Get().GetEditor()->GetViewport().GetRenderTarget().getSize().x);
				lastCharacter->SetRecalculateVisuals(false);
				lastCharacter->SetCharacter(block.character);
			}
			std::string content = block.content;
			if (block.type == TextBlock::Parenthetical)
			{
				content = "(" + content + ")";
			}
			if (!lastCharacter->GetText().empty())
			{
				content = " " + content;
			}

			lastCharacter->AppendText(content);
			wasLastDialogue = true;
			continue;
		}

		wasLastDialogue = false;

		if (lastCharacter != nullptr)
			lastCharacter->SetRecalculateVisuals(true);

		lastCharacter = nullptr;

		if (block.type == TextBlock::Action)
		{
			LOG("Action not yet implemented");
		}
		if (block.type == TextBlock::Slug)
		{
			LOG("Slug not yet implemented");
		}
	}

	delete script;
}

void Project::RenderTo(sf::RenderTexture* renderTarget)
{
	uint32_t offset = 0;
	for (auto& block : m_blocks)
	{
		block->SetOffset(offset);
		block->RenderTo(renderTarget);
	}
}

void Project::Resize(uint32_t width, uint32_t height)
{
	for (auto& block : m_blocks)
	{
		block->ChangeWidth(width);
	}
}
