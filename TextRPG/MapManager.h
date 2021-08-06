#pragma once
#include"Game.h"
class CMapManager
{
	CMapManager();
	~CMapManager();
private :
	class CMap* m_Maps[MT_End];
public :
	Map_Menu Menu();
private:
	static CMapManager* m_pInst;
public:
	static CMapManager* GetInst()
	{
		if (m_pInst == nullptr) m_pInst = new CMapManager;
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

