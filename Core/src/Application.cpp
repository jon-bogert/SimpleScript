#include "Application.h"
#include "Project.h"

#include <iostream>

void Application::Start()
{
	m_windowManager.Start(1280, 720);
	m_windowManager.SetResizeCallback([](uint32_t w, uint32_t h) {std::cout << "resize: " << w << " " << h << '\n'; });

	m_editor = std::make_unique<Editor>();
	m_windowManager.SetEditor(m_editor.get());
}

void Application::Update()
{
	while (m_windowManager.IsOpen())
	{
		m_windowManager.Update();
	}
}

void Application::Shutdown()
{
	m_windowManager.Shutdown();
}

Project* Application::GetActiveProject()
{
	return m_activeProject.get();
}

const Project* Application::GetActiveProject() const
{
	return m_activeProject.get();;
}
