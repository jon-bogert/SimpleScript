#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include "Application.h"

#include "Scene.h"

int main()
{
	//Application::Get().Start();
	//Application::Get().Update();
	//Application::Get().Shutdown();

	Scene scene;
	scene.Load("example.yaml", SceneEntry());
	scene.Export("example.docx");
}