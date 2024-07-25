#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include "Application.h"

#include "Script.h"

#include "GUI/CharacterBlock.h"
#include "Character.h"

void TestLoadExport()
{
	Script scene;
	scene.Load("example.yaml");
	scene.Export("example.docx");
}

void TestCharacterBox()
{
	Character c;
	c.color = { 151, 153, 47, 255 };
	c.name = "THREEPIO";
	c.notes = "This is a test character";

	CharacterBlock block;
	block.SetCharacter(c.name);
	block.Start(700);
	//block.SetText("Out of the shadows of a dingy side-building limps Owen Lars, a large burly man in his mid-fifties. His reddish eyes are sunken in a dust-covered face. As the farmer carefully inspects each robot, he is closely followed by his slump shouldered nephew, Luke Skywalker. One of the vile little Jawas walks ahead of the farmer spouting an animated sales pitch in a queer, unintelligible language.");
	block.SetText("He says he's the property of Obi-Wan Kenobi, a resident of these parts. And it's a private message for him. Quite frankly, sir I don't know what he's talking about. Our last master was Captain Antilles, but with what we've been through, this little R2 unit has become a bit eccentric.");

	sf::RenderTexture target;
	target.create(1280, 720);
	sf::Sprite s;
	s.setTexture(target.getTexture());

	sf::RenderWindow window(sf::VideoMode(1280, 720), "Hello Block");
	while (window.isOpen())
	{
		target.clear(sf::Color::Black);
		block.RenderTo(&target);
		target.display();

		window.clear(sf::Color::White);
		window.draw(s);
		window.display();
	}
}

int main()
{
	Application::Get().Start();
	Application::Get().Update();
	Application::Get().Shutdown();

	//TestCharacterBox();
}