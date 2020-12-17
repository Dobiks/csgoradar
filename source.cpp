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
	DWORD currentPlayer;
}val;

struct localPlayer
{
	float x;
	float y;
	int team;
}localPlayer;

struct gameOffsets
{
	DWORD dwLocalPlayer = 0xD8722C;
	DWORD dwForceJump = 0x5248A34;
	DWORD m_fFlags = 0x104;
	DWORD m_vecOrigin = 0x138;
	DWORD dwEntityList = 0x4D9EAD4;
	DWORD m_bDormant = 0xED;
	DWORD m_iTeamNum = 0xF4;
	DWORD m_iHealth = 0x100;;

}offsets;

struct player
{
	float x;
	float y;
	int isDormant;
	int team;
	float health;
};

void makeWindowOnTop(sf::RenderWindow& window)
{
	HWND hwnd = window.getSystemHandle();
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

player add(float x, float y, float health, int team, std::string map)
{
	player tmp;
	if (map == "mirage.png")
	{
		tmp.x = 215 + x / 15;
		tmp.y = 110 - y / 15;
	}
	else if (map == "dust.png")
	{
		tmp.x = 185 + x / 12;
		tmp.y = 250 - y / 12;
	}
	tmp.health = health;
	tmp.team = team;
	return tmp;
}

int main()
{
	std::cout << "1. Mirage" << std::endl << "2. Dust 2" << std::endl;
	int pick;
	std::cin >> pick;
	std::string map = "";
	switch (pick)
	{
	case 1:
		map = "mirage.png";
		break;
	case 2:
		map = "dust.png";
		break;
	}
	sf::RenderWindow window(sf::VideoMode(340, 340), "Google Chrome haha");
	sf::Texture t1;
	t1.loadFromFile(map);
	sf::Sprite s(t1);
	makeWindowOnTop(window);
	window.setPosition(sf::Vector2i(0, 100));
	while (window.isOpen())
	{
		std::vector<player>players;
		val.process = MemClass.getProcess("csgo.exe");
		val.gameModule = MemClass.getModule(val.process, "client.dll");
		val.localPlayer = MemClass.readMem<DWORD>(val.gameModule + offsets.dwLocalPlayer);
		localPlayer.team = MemClass.readMem<int>(val.localPlayer + offsets.m_iTeamNum);
		localPlayer.x = MemClass.readMem<float>(val.localPlayer + offsets.m_vecOrigin);
		localPlayer.y = MemClass.readMem<float>(val.localPlayer + offsets.m_vecOrigin + 0x4);
		system("cls");
		std::cout << "x = " << localPlayer.x << std::endl;
		std::cout << "y = " << localPlayer.y << std::endl;
		players.push_back(add(localPlayer.x, localPlayer.y, 100, 5, map));

		for (int i = 0; i < 32; i++)
		{
			player tmp;
			if (i > 0) val.currentPlayer = MemClass.readMem<DWORD>(val.gameModule + offsets.dwEntityList + i * 0x10);
			tmp.isDormant = MemClass.readMem<DWORD>(val.currentPlayer + offsets.m_bDormant);
			if (tmp.isDormant == 0)
			{
				tmp.x = MemClass.readMem<float>(val.currentPlayer + offsets.m_vecOrigin);
				tmp.y = MemClass.readMem<float>(val.currentPlayer + offsets.m_vecOrigin + 0x4);
				tmp.health = MemClass.readMem<DWORD>(val.currentPlayer + offsets.m_iHealth);
				tmp.team = MemClass.readMem<DWORD>(val.currentPlayer + offsets.m_iTeamNum);
				players.push_back(add(tmp.x, tmp.y, tmp.health, tmp.team, map));
			}
		}


		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		window.clear();
		window.draw(s);
		for (auto el : players)
		{
			sf::CircleShape shape(5);
			shape.setPosition(el.x, el.y);
			if (el.team == 5)
			{
				shape.setFillColor(sf::Color(0, 0, 250));
				window.draw(shape);
			}
			else if (el.team != localPlayer.team)
			{
				shape.setFillColor(sf::Color(250, 0, 0));
				window.draw(shape);
			}
		}
		window.display();
	}
	return 0;
}