#pragma once
#include "WindowManager.h"

class ContentWindow : public Window
{
public:
	void Start() override;
	void Specs() override;
	void OnGUI() override;

private:
};