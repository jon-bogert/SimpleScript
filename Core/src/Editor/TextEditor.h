#pragma once

#include "GUI/Cursor.h"
#include "GUI/RenderObject.h"

class TextEditor
{
public:
	void RenderTo(sf::RenderTexture* renderTarget);

	void SetCursorFromPointer();

	//void CursorForward();
	//void CursorBackward();
	//
	//void CursorForwardWord();
	//void CursorBackwardWord();

private:
	void CoordAndLineData();

	RenderObject* m_currentObject = nullptr;
	uint32_t m_characterPos = 0;

	std::vector<sf::Vector2f> m_coordInfo;	// stores the coordinates of each letter
	std::vector<size_t> m_lineInfo;			// Stores the index of the first letter of a new line

	Cursor m_cursor;
};