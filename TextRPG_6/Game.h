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
	MT_Hard,
	MT_End
};

enum class Map_Menu
{
	None,
	Easy,
	Normal,
	Hard
};

enum class Job
{
	None,
	Knight,
	Archer,
	Magician,
	End
};

enum class Battle_Menu
{
	None,
	Attack,
	Back
};

enum class Battle_Result
{
	None,
	Player_Death,
	Monster_Death
};

enum class Store_Menu
{
	None,
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

enum Equip_Type
{
	Equip_Weapon,
	Equip_Armor,
	Equip_End
};