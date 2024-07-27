#pragma once

#include "Viewport.h"
#include "TextEditor.h"

#include <imgui.h>
#include <imgui-SFML.h>

class Editor
{
public:
	void Update();

	Viewport& GetViewport();
	TextEditor& GetTextEditor();

private:
	Viewport m_viewport;
	TextEditor m_textEditor;
};