#pragma once
#include"Game.h"
class CGameManager
{
	CGameManager();
	~CGameManager();
public :
	Main_Menu Menu();
	void Run();
	bool Init();
private :
	static CGameManager* m_pInst;
public :
	static CGameManager* GetInst() 
	{
		if (m_pInst == nullptr) m_pInst = new CGameManager;
		return m_pInst;
	}
	static void DestoryInst()
	{
		if (m_pInst) delete m_pInst;
	}
};

