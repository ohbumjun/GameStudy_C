#include<iostream>
#include"GameManager.h"

using namespace std;

int main()
{
	if (!CGameManager::GetInst()->Init())
	{
		CGameManager::GetInst()->DestroyInst();
		return true;
	}

	CGameManager::GetInst()->Run();
	CGameManager::GetInst()->DestroyInst();

	return 0;
}