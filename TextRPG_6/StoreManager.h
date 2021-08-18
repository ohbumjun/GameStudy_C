#pragma once
#include"Game.h"
class CStoreManager
{
public:
	void Run();
	bool Init();
	Store_Menu Menu();
private:
	static CStoreManager* m_pInst;
public:
	static CStoreManager* GetInst()
	{
		if (m_pInst == nullptr)
			m_pInst = new CStoreManager;
		return m_pInst;
	}
	static void DestroyInst()
	{
		if (m_pInst) delete m_pInst;
	}
};

