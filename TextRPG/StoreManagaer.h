#pragma once
class CStoreManagaer
{
	CStoreManagaer();
	~CStoreManagaer();
public:
	void Run();
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

