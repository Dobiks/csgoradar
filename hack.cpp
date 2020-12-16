#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <iostream>
#include<vector>
#include<string>
#include "MemMan.h"
MemMan MemClass;

struct values
{
	DWORD localPlayer;
	DWORD process;
	DWORD gameModule;
	BYTE flag;
}val;

struct localPlayerPos
{
	float x;
	float y;
}pos;

struct gameOffsets
{
	DWORD dwLocalPlayer = 0xD8722C;
	DWORD dwForceJump = 0x5248A34;
	DWORD m_fFlags = 0x104;
	DWORD m_vecOrigin = 0x138;
}offsets;


std::vector<sf::CircleShape>add_circ(int cond)
{
	std::vector<sf::CircleShape>tmp;
	for (int i = 0; i < 5; i++)
	{
		sf::CircleShape shape(5);
		if (cond == 0)
		{
			shape.setFillColor(sf::Color(250, 0, 0));

		}
		else
		{
			shape.setFillColor(sf::Color(0, 250, 0));
		}
		tmp.push_back(shape);
	}
	return tmp;
}




int main()
{
	sf::RenderWindow window(sf::VideoMode(340, 340), "CziternikMiki");
	sf::Texture t1;
	t1.loadFromFile("mirage.png");
	sf::Sprite s(t1);
	std::vector<sf::CircleShape> enemy = add_circ(0);
	std::vector<sf::CircleShape>ally = add_circ(1);

	val.process = MemClass.getProcess("csgo.exe");
	val.gameModule = MemClass.getModule(val.process, "client.dll");
	
	val.localPlayer = MemClass.readMem<DWORD>(val.gameModule + offsets.dwLocalPlayer);
	std::cout << std::hex << val.localPlayer<< std::endl;
	std::cout << std::hex << val.localPlayer + offsets.m_vecOrigin << std::endl;
	//system("pause");
	while (window.isOpen())
	{
		pos.x = MemClass.readMem<float>(val.localPlayer + offsets.m_vecOrigin);
		pos.y = MemClass.readMem<float>(val.localPlayer + offsets.m_vecOrigin + 0x4);
		std::cout << std::dec<<pos.x << std::endl;
		std::cout << std::dec<<pos.y << std::endl;
		system("cls");
		Sleep(1);
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		pos.x = 215 + pos.x/15;
		pos.y = 110 - pos.y / 15;
		enemy[0].setPosition( pos.x,pos.y);

		//draw
		window.clear();
		window.draw(s);
		for (auto el : enemy)
		{
			window.draw(el);
		}
		for (auto el : ally)
		{
			window.draw(el);
		}
		window.display();
	}
	return 0;
}
