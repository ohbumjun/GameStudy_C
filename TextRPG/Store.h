#pragma once
#include"Game.h"
class CStore
{
public :
	CStore();
	~CStore();
private :
	Store_Type m_Type;
	class CItem* m_Item[IT_End];
public :
	bool Init(Store_Type Type);
	void Run();
	int Menu(Store_Type Type);
};

