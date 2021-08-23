#pragma once
#include"Game.h"
class CMapManager
{
private :
	CMapManager();
	~CMapManager();
public :
	class CMap* m_Map[MT_End];
public:
	void Run();
	bool Init();
	int Menu();
private:
	static CMapManager* m_pInst;
public:
	static CMapManager* GetInst()
	{
		if (m_pInst == nullptr)
			m_pInst = new CMapManager;
		return m_pInst;
	}
	static void DestroyInst()
	{
		if (m_pInst) delete m_pInst;
	}
};

