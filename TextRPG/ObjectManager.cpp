#include "ObjectManager.h"
#include"Player.h"
#include"Monster.h"

CObjectManager* CObjectManager::m_pInst = nullptr;
CObjectManager::CObjectManager() : m_DropItem{}
{
}

CObjectManager::~CObjectManager()
{
}

bool CObjectManager::Init()
{
	m_Player = new CPlayer;
	if (!m_Player->Init()) return false;
	return true;
}

void CObjectManager::Run()
{
}
