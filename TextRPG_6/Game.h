#pragma once
#include<iostream>

#define INVENTORY_MAX 10
#define LEVEL_MAX 10

enum class Main_Menu
{
	None,
	Map,
	Store,
	Inventory,
	Exit
};

enum Map_Type
{
	MT_Easy,
	MT_Normal,
	MT_Hard
};

enum class Map_Menu
{
	Easy,
	Normal,
	Hard
};

enum class Job
{
	None,
	Knight,
	Archer,
	Magician
};

enum class Battle_Menu
{
	None,
	Attack,
	Back
};

enum class Battle_Result
{
	Player_Death,
	Monster_Death
};

enum class Store_Menu
{
	Weapon,
	Armor
};

enum Store_Type
{
	ST_Weapon,
	ST_Armor,
	ST_End
};

enum Item_Type
{
	IT_Weapon,
	IT_Armor,
	IT_End
};