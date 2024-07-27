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
	size_t end = m_activeBlocksBegin + m_activeBlocksSize;
	for (size_t i = m_activeBlocksBegin; i < end; ++i)
	{
		m_blocks[i]->RenderTo(renderTarget);
	}

	//sf::Text debugText;
	//debugText.setFont(Style::Get().BodyFont());
	//debugText.setCharacterSize(10);
	//debugText.setFillColor({ 255, 0, 255, 255 });
	//debugText.setString("Begin: " + std::to_string(m_activeBlocksBegin)
	//	+ "\nSize: " + std::to_string(m_activeBlocksSize)
	//    + "\nHeight: " + std::to_string(m_blocks[m_activeBlocksBegin]->GetHeight())
	//	+ "\nOffset: " + std::to_string(m_scrollOffset));
	//sf::RectangleShape debugBG;
	//debugBG.setSize(debugText.getLocalBounds().getSize() + sf::Vector2f(5.f, 5.f));
	//debugBG.setFillColor({ 0, 0, 0, 195 });
	//
	//renderTarget->draw(debugBG);
	//renderTarget->draw(debugText);
}

void Project::Resize(uint32_t width, uint32_t height)
{
	RenderObject::ChangeWidth(width);
	m_viewportHeight = height;
	OnScroll(0.f, true);
}

void Project::OnScroll(float amt, bool doRecalc)
{
	m_scrollOffset -= amt * m_scrollFactor;
	if (m_activeBlocksBegin == 0 && m_scrollOffset < 0.f)
		m_scrollOffset = 0.f;

	float blockGap = Style::Get().BlockGap();

	// Check beginning
	if (m_scrollOffset < 0.f)
	{
		--m_activeBlocksBegin;
		++m_activeBlocksSize;
		m_blocks[m_activeBlocksBegin]->Recalculate();
		m_scrollOffset += m_blocks[m_activeBlocksBegin]->GetHeight() + blockGap;
	}
	if (m_scrollOffset >= m_blocks[m_activeBlocksBegin]->GetHeight() + blockGap)
	{
		m_scrollOffset -= m_blocks[m_activeBlocksBegin]->GetHeight() + blockGap;
		++m_activeBlocksBegin;
		--m_activeBlocksSize;
	}

	// Trim the end
	float currY = -m_scrollOffset;
	for (size_t i = 0; i < m_activeBlocksSize; ++i)
	{
		if (currY > m_viewportHeight)
		{
			m_activeBlocksSize = i;
			break;
		}
		if (doRecalc)
		{
			m_blocks[m_activeBlocksBegin + i]->Recalculate();
		}

		m_blocks[m_activeBlocksBegin + i]->SetYPosition(currY);
		currY += (float)m_blocks[m_activeBlocksBegin + i]->GetHeight() + blockGap;
	}

	//Extend the end
	while (currY < m_viewportHeight)
	{
		size_t next = m_activeBlocksBegin + m_activeBlocksSize;
		if (next >= m_blocks.size()) // prevent going past end
		{
			float endDelta = m_viewportHeight - currY;
			OnScroll(endDelta / m_scrollFactor);
			return;
		}

		++m_activeBlocksSize;
		m_blocks[next]->Recalculate();
		m_blocks[next]->SetYPosition(currY);
		currY += (float)m_blocks[next]->GetHeight() + blockGap;
	}
}

std::vector<RenderObject*> Project::GetActiveBlocks()
{
	std::vector<RenderObject*> result(m_activeBlocksSize);
	for (size_t i = 0; i < m_activeBlocksSize; ++i)
	{
		result[i] = m_blocks[m_activeBlocksBegin + i].get();
	}
	return result;
}
