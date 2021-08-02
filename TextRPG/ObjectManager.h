#pragma once
#include"Game.h"

class CObjectManager
{
	CObjectManager();
	~CObjectManager();
public:
	bool Init();
	void Run();
private :
	class CPlayer* m_Player;
	class CMonster* m_Monster[3];
	int m_LevelUpExp[LEVEL_MAX];
	class CItem* m_DropItem[6];
public :
	int GetLevelUpExp(int Level) { return m_LevelUpExp[Level-1]; }
	class CItem* GetDropItem(int Index) { return m_DropItem[Index]; }
	class CPlayer* GetPlayer() { return m_Player; }
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
		if (m_pInst)
			delete m_pInst;
	}
};

