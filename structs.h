#pragma once
#include <iostream>
#include <Windows.h>

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

struct player
{
	float x;
	float y;
	int isDormant;
	int team;
	float health;
};


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
