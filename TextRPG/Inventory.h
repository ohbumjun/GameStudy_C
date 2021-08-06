#pragma once
#include"Game.h"

class CInventory
{
	CInventory();
	~CInventory();
private:
	static CInventory* m_pInst;
public:
	static CInventory* GetInst()
	{
		if (m_pInst == nullptr) m_pInst = new CInventory;
		return m_pInst;
	}
	static void DestroyInst()
	{
		if (m_pInst) delete m_pInst;
	}
public:
	bool Init();
	void Run();
};

