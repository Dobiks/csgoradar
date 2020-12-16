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

player add(float x, float y, float health, int team)
{
	player tmp;
	tmp.x = 215 + x / 15;
	tmp.y = 110 - y / 15;
	tmp.health = health;
	tmp.team = team;
	return tmp;
}
int main()
{
	sf::RenderWindow window(sf::VideoMode(340, 340), "CziternikMiki");
	sf::Texture t1;
	t1.loadFromFile("mirage.png");
	sf::Sprite s(t1);

	while (window.isOpen())
	{
		std::vector<player>players;
		val.process = MemClass.getProcess("csgo.exe");
		val.gameModule = MemClass.getModule(val.process, "client.dll");
		val.localPlayer = MemClass.readMem<DWORD>(val.gameModule + offsets.dwLocalPlayer);
		localPlayer.team = MemClass.readMem<int>(val.localPlayer + offsets.m_iTeamNum);
		localPlayer.x = MemClass.readMem<float>(val.localPlayer + offsets.m_vecOrigin);
		localPlayer.y = MemClass.readMem<float>(val.localPlayer + offsets.m_vecOrigin + 0x4);

		//players.push_back(add(localPlayer.x, localPlayer.y, 100, 5));

		for (int i = 0; i < 32; i++)
		{
			player tmp;
			if (i > 0) val.currentPlayer = MemClass.readMem<DWORD>(val.gameModule + offsets.dwEntityList + i * 0x10);
			tmp.isDormant= MemClass.readMem<DWORD>(val.currentPlayer + offsets.m_bDormant);
			if (tmp.isDormant == 0)
			{
				tmp.x = MemClass.readMem<float>(val.currentPlayer + offsets.m_vecOrigin);
				tmp.y = MemClass.readMem<float>(val.currentPlayer + offsets.m_vecOrigin + 0x4);
				tmp.health = MemClass.readMem<DWORD>(val.currentPlayer + offsets.m_iHealth);
				tmp.team = MemClass.readMem<DWORD>(val.currentPlayer + offsets.m_iTeamNum);
					players.push_back(add(tmp.x, tmp.y, tmp.health, tmp.team));

				

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
			}
			else if (el.team == 3)
			{
				shape.setFillColor(sf::Color(0, 250, 0));
			}
			else
			{
				shape.setFillColor(sf::Color(250, 0, 0));
			}
			window.draw(shape);
		}

		window.display();

	}

	return 0;
}