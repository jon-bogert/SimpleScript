#pragma once

#include "../WindowManager.h"

class TextBlock;

class InspectorWindow : public Window
{
public:
	void Start() override;
	void OnGUI() override;

private:
	bool m_editCharacter = false;
	char m_editCharacterBuffer[256];
};