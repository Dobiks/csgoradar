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
	DWORD team;
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

struct Player
{
	float x;
	float y;
	DWORD isDormant;
	DWORD team;
	DWORD health;
};

std::vector<sf::CircleShape>add_circ(int cond)
{
	std::vector<sf::CircleShape>tmp;
	if (cond == 0) {
		for (int i = 0; i < 16; i++)
		{
			sf::CircleShape shape(5);
			shape.setFillColor(sf::Color(250, 0, 0));
			tmp.push_back(shape);
		}
	}
	else
	{
		for (int i = 0; i < 16; i++)
		{
			sf::CircleShape shape(5);
			shape.setFillColor(sf::Color(0, 250, 0));
			tmp.push_back(shape);
		}
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
	std::vector<sf::CircleShape>ally = add_circ(1); //a

	val.process = MemClass.getProcess("csgo.exe");
	val.gameModule = MemClass.getModule(val.process, "client.dll");

	val.localPlayer = MemClass.readMem<DWORD>(val.gameModule + offsets.dwLocalPlayer);
	std::cout << std::hex << val.localPlayer << std::endl;
	std::cout << std::hex << val.localPlayer + offsets.m_vecOrigin << std::endl;

	localPlayer.team = MemClass.readMem<DWORD>(val.localPlayer + offsets.m_iTeamNum);
	//system("pause");

	while (window.isOpen())
	{

		//exit
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		//lc pos
		localPlayer.x = MemClass.readMem<float>(val.localPlayer + offsets.m_vecOrigin);
		localPlayer.y = MemClass.readMem<float>(val.localPlayer + offsets.m_vecOrigin + 0x4);
		localPlayer.x = 215 + localPlayer.x / 15;
		localPlayer.y = 110 + localPlayer.y / 15;

		//ally[0].setPosition(localPlayer.x, localPlayer.y);

		//player struct vector
		std::vector<Player>players_vector;

		//players pos
		int licznik_ally = 0;
		int licznik_enemy = 0;
		for (int i = 0; i < 32; i++)
		{
			Player tmp;
			players_vector.push_back(tmp);
			val.currentPlayer = MemClass.readMem<DWORD>(val.gameModule + offsets.dwEntityList + i * 0x10);
			players_vector[i].isDormant = MemClass.readMem<DWORD>(val.currentPlayer + offsets.m_bDormant);
			players_vector[i].team = MemClass.readMem<DWORD>(val.currentPlayer + offsets.m_iTeamNum);
			players_vector[i].health = MemClass.readMem<DWORD>(val.currentPlayer + offsets.m_iHealth);

			if (players_vector[i].isDormant == 0)
			{
				players_vector[i].x = MemClass.readMem<float>(val.currentPlayer + offsets.m_vecOrigin);
				players_vector[i].y = MemClass.readMem<float>(val.currentPlayer + offsets.m_vecOrigin + 0x4);
				players_vector[i].x = 215 + players_vector[i].x / 15;
				players_vector[i].y = 110 - players_vector[i].y / 15;

				if (licznik_ally < 15 && licznik_enemy < 15) 
				{
					if (players_vector[i].team == 3 && players_vector[i].health > 0)
					{
						//ally

						ally[licznik_ally].setPosition(players_vector[i].x, players_vector[i].y);
						licznik_ally++;
					}
					else if (players_vector[i].team == 2 && players_vector[i].health > 0)
					{
						//enemy
						enemy[licznik_enemy].setPosition(players_vector[i].x, players_vector[i].y);
						licznik_enemy++;

					}
				}

			}
		}
		//set pos


		//draw
		window.clear();
		window.draw(s);
		for (auto el : enemy)
		{
			if (el.getPosition().x != 0 || el.getPosition().y != 0)
			{
				window.draw(el);
			}
		}
		for (auto el : ally)
		{
			if (el.getPosition().x != 0 || el.getPosition().y != 0)
			{
				if (el.getPosition().x == localPlayer.x)
				{
					el.setFillColor(sf::Color(1, 1, 250));
				}
				window.draw(el);
			}
		}
		window.display();
		Sleep(1);
	}
	
	return 0;
}