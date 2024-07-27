#include "TextEditor.h"

#include "Application.h"
#include "Debug.h"

void TextEditor::RenderTo(sf::RenderTexture* renderTarget)
{
	if (m_currentObject == nullptr)
		return;

	m_cursor.RenderTo(renderTarget);
}

void TextEditor::SetCursorFromPointer()
{
	sf::Vector2f mouse = Application::Get().GetEditor()->GetViewport().GetMousePosition();
	std::vector<RenderObject*> activeObjects = Application::Get().GetActiveProject()->GetActiveBlocks();

	m_currentObject = nullptr;

	for (RenderObject* object : activeObjects)
	{
		if (object->PointCheck(mouse))
		{
			m_currentObject = object;
			break;
		}
	}

	if (m_currentObject)
		return;

	// Find letter
	CoordAndLineData();

	size_t closestIndex = 0;
	float closestSqrMag = FLT_MAX;

	for (size_t i = 0; i < m_coordInfo.size(); ++i)
	{
		sf::Vector2f offset = m_coordInfo[i] - mouse;
		float sqrMag = (offset.x * offset.x) - (offset.y * offset.y);
		if (sqrMag < closestSqrMag)
		{
			closestSqrMag = sqrMag;
			closestIndex = i;
		}
	}

	m_cursor.SetPosition(m_coordInfo[closestIndex]);
	m_characterPos = closestIndex;
}

void TextEditor::CoordAndLineData()
{
	if (m_currentObject)
	{
		LOG("TextEditor::CoordAndLineData - Why is m_currentObject null?");
		return;
	}

	std::string contents = m_currentObject->GetContent();
	sf::Text testField = m_currentObject->GetField(); // Copy on purpose;

	m_coordInfo.clear();
	m_lineInfo.clear();

	std::string lineSoFar;
	for (size_t i = 0; i < contents.length(); ++i)
	{
		sf::Vector2f& coord = m_coordInfo.emplace_back();
		coord.x = testField.getGlobalBounds().left + testField.getGlobalBounds().width;

		if (contents[i] == '\n')
		{
			lineSoFar.clear();
			sf::Vector2f& nlCoord = m_coordInfo.emplace_back();
			nlCoord.x = coord.x + 5;
			m_lineInfo.push_back(i + 1);
			++i;
		}
		else
		{
		    lineSoFar.push_back(contents[i]);
		}

		lineSoFar.clear();
		testField.setString(lineSoFar);

		if (m_lineInfo.empty())
			return;

		size_t nextLine = 0;

		for (size_t i = 0; i < contents.length(); ++i)
		{
			if (nextLine < m_lineInfo.size() && i == m_lineInfo[i])
			{
				lineSoFar.append("0\n");
				testField.setString(lineSoFar);
				++nextLine;
			}

			m_coordInfo[i].y = testField.getGlobalBounds().top + testField.getGlobalBounds().height;
		}
	}
}


