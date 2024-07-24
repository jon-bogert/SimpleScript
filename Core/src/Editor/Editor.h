#pragma once

#include "Viewport.h"

#include <imgui.h>
#include <imgui-SFML.h>

class Editor
{
public:
	void Update();

	Viewport& GetViewport();

private:
	Viewport m_viewport;
};