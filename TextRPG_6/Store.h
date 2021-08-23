#pragma once
#include"Game.h"

class CStore
{
public :
	CStore();
	~CStore();
private :
	Store_Type m_Type;
	class CItem* m_Items[3];
public :
	bool Init(Store_Type Type);
	void Run();
	int Menu(class CPlayer* player);
};

