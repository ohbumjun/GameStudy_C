#pragma once

#include<iostream>
#include<time.h>

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

// Job
enum class Job
{
	Knight,
	Archer,
	Magician
};


// Store 
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

// Map 
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

// Battle 
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

// Item 
enum Item_Type
{
	IT_Weapon,
	IT_Armor,
	IT_End
};

// Equip
enum Equip_Type
{
	Equip_Weapon,
	Equip_Armor,
	Equip_End
};

