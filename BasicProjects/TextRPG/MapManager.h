#pragma once
#include"Game.h"
class CMapManager
{
	CMapManager();
	~CMapManager();
private :
	class CMap* m_Map[3];
public:
	void Run();
	Map_Menu Menu();
	bool Init();
private:
	static CMapManager* m_pInst;
public:
	static CMapManager* GetInst()
	{
		if (m_pInst == nullptr) m_pInst = new CMapManager;
		return m_pInst;
	}
	static void DestoryInst()
	{
		if (m_pInst) delete m_pInst;
	}
};

