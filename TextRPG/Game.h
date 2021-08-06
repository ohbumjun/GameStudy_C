#pragma once

#include<iostream>
#include<time.h>

#define LEVEL_MAX 10
#define INVENTORY_MAX 10

enum class Main_Menu
{
	None,
	Map,
	Store,
	Inventory,
	End
};

enum class Map_Menu
{
	None,
	Easy,
	Normal,
	Hard,
	End
};

enum Map_Type
{
	MT_Easy,
	MT_Normal,
	MT_Hard,
	MT_End
};

enum class Job
{
	Knight,
	Archer,
	Magicion
};

enum Store_Type
{
	ST_Weapon,
	ST_Armor,
	ST_End
};

enum class Store_Menu
{
	Weapon,
	Armor
};

enum Item_Type
{
	IT_Weapon,
	IT_Armor,
	IT_End
};


enum class Battle_Menu
{
	None,
	Attack,
	Armor,
	End
};

enum class Battle_Result
{
	Player_Death,
	Monster_Death
};