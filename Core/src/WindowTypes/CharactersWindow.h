#pragma once

#include "../WindowManager.h"

class CharactersWindow : public Window
{
public:
	void Start() override;
	void OnGUI() override;

private:
	void Search();
	void Edit();
	void Info();

	std::string m_selected = "";
	char m_nameSearchBuffer[256];
	char m_nameChangeBuffer[256];
	bool m_editName = false;
	bool m_editOrder = false;
};