#pragma once
#include"Game.h"
class CObjectManager
{
private :
	class CPlayer* m_Player;
	class CMonster* m_Monster[3];
public :
	class CPlayer* GetPlayer()
	{
		return m_Player;
	}
public:
	void Run();
	bool Init();
private:
	static CObjectManager* m_pInst;
public:
	static CObjectManager* GetInst()
	{
		if (m_pInst == nullptr)
			m_pInst = new CObjectManager;
		return m_pInst;
	}
	static void DestroyInst()
	{
		if (m_pInst) delete m_pInst;
	}
};

