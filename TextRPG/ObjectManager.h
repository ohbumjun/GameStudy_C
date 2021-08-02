#pragma once
class CObjectManager
{
	CObjectManager();
	~CObjectManager();
public:
	bool Init();
	void Run();
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

