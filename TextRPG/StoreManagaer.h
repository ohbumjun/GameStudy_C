#pragma once
#include"Game.h"
class CStoreManagaer
{
	CStoreManagaer();
	~CStoreManagaer();
private :
	class CStore* m_Store[ST_End];
public:
	void Run();
	Store_Menu Menu();
	bool Init();
private:
	static CStoreManagaer* m_pInst;
public:
	static CStoreManagaer* GetInst()
	{
		if (m_pInst == nullptr) m_pInst = new CStoreManagaer;
		return m_pInst;
	}
	static void DestoryInst()
	{
		if (m_pInst) delete m_pInst;
	}
};

