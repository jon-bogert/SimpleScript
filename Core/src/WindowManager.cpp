#include "WindowManager.h"

#include <imgui.h>

WindowManager& WindowManager::Get()
{
	static WindowManager inst = WindowManager();
	return inst;
}

void WindowManager::PreUpdate()
{
	for (std::unique_ptr<Window>& w : Get().m_windows)
	{
		w->PreUpdate();
	}
}
void WindowManager::OnGUI()
{
	
	for (std::unique_ptr<Window>& w : Get().m_windows)
	{
		if (!w->isOpen)
			continue;

		w->Specs();
		ImGui::Begin(w->name.c_str(), &w->isOpen, w->windowFlags);
		w->OnGUI();
		ImGui::End();
	}
}
void WindowManager::PostUpdate()
{
	for (auto it = Get().m_windows.begin(); it != Get().m_windows.end();)
	{
		//if (!it->get()->isOpen) // remove closed windows
		//{
		//	it->get()->Destroy();
		//	it = Get().m_windows.erase(it);
		//	continue;
		//}
		it->get()->PostUpdate();
		it++;
	}
}
void WindowManager::Destroy()
{
	for (std::unique_ptr<Window>& w : Get().m_windows)
	{
		w->Destroy();
	}
}