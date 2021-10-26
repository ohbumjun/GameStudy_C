#include"GameManager.h"

int main()
{
	if (!CGameManager::GetInst()->Init())
	{
		CGameManager::GetInst()->DestoryInst();
		return 0;
	}
	CGameManager::GetInst()->Run();
	CGameManager::GetInst()->DestoryInst();
	return 0;
}