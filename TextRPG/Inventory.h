#pragma once
class CInventory
{
	CInventory();
	~CInventory();
public:
	void Run();
	bool Init();
private:
	static CInventory* m_pInst;
public:
	static CInventory* GetInst()
	{
		if (m_pInst == nullptr) m_pInst = new CInventory;
		return m_pInst;
	}
	static void DestoryInst()
	{
		if (m_pInst) delete m_pInst;
	}
};

