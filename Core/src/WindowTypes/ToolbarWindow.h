#pragma once

#include "../WindowManager.h"
#include "TextBlock.h"

class ToolbarWindow : public Window
{
public: 
	void Start() override;
	void OnGUI() override;

private:
	void MoveUp();
	void MoveDown();
	void Remove();
	void Add(TextBlock::Type type);

	bool m_showCharacterAdd = false;
	char m_characterAddBuffer[256];
};