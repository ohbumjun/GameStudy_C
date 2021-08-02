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

enum class Job
{
	None,
	Knight,
	Archer,
	Magicion,
	End
};

enum class Store_Menu
{
	None,
	Weapon,
	Armor,
	Back
};

enum Store_Type
{
	ST_Weapon,
	ST_Armor,
	ST_End
};

enum class Map_Menu
{
	None,
	Easy,
	Normal,
	Hard,
	Back
};

enum Map_Type
{
	MT_Easy,
	MT_Normal,
	MT_Hard,
	MT_End
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
	Monster_Death,
	None
};

enum Equip_Type
{
	Equip_Weapon,
	Equip_Armor,
	Equip_End
};

enum Item_Type
{
	IT_Weapon,
	IT_Armor,
	IT_End
};