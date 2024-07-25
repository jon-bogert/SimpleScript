#include "Application.h"
#include "Project.h"

#include <iostream>

void Application::Start()
{
	m_windowManager.Start(1280, 720);
	m_windowManager.SetResizeCallback([](uint32_t w, uint32_t h) {std::cout << "resize: " << w << " " << h << '\n'; });

	m_editor = std::make_unique<Editor>();
	m_windowManager.SetEditor(m_editor.get());

	// TEMP
	m_activeProject = std::make_unique<Project>();
	m_activeProject->Start();
	m_activeProject->Load("example.yaml");
	std::cout << "Done Load" << std::endl;
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

void Application::UpdateScroll(float amt)
{
	m_scroll -= amt;
	
	if (m_scroll >= 0.f)
		m_scroll = 0.f;
}

Project* Application::GetActiveProject()
{
	return m_activeProject.get();
}

const Project* Application::GetActiveProject() const
{
	return m_activeProject.get();;
}
