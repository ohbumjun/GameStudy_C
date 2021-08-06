#include"GameManager.h"

int main()
{
	if (!CGameManager::GetInst()->Init())
	{
		CGameManager::GetInst()->DestroyInst();
		return 0;
	}

	CGameManager::GetInst()->Run();
	CGameManager::GetInst()->DestroyInst();

	return 0;
}

