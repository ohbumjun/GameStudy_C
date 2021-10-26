#pragma once
#include"Game.h"
class CGameManager
{
public :
	void Run();
	bool Init();
	Main_Menu Menu();
private :
	static CGameManager* m_pInst;
public :
	static CGameManager* GetInst()
	{
		if (m_pInst == nullptr)
			m_pInst = new CGameManager;
		return m_pInst;
	}
	static void DestroyInst()
	{
		if (m_pInst) delete m_pInst;
	}
};

