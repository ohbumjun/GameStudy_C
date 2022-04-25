
#include "MainSceneMode.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/Viewport.h"
#include "../Object/Player.h"

CMainSceneMode::CMainSceneMode()
{
	SetTypeID<CMainSceneMode>();
}

CMainSceneMode::~CMainSceneMode()
{
}

bool CMainSceneMode::Init()
{
	LoadMesh();

	CreateMaterial();

	CreateAnimationSequence();

	CreateParticle();

	if (m_LoadingFunction)
		m_LoadingFunction(false, 0.3f);

	CPlayer* Player = m_Scene->CreateGameObject<CPlayer>("Player");

	SetPlayerObject(Player);

	
	return true;
}

void CMainSceneMode::LoadMesh()
{
	// m_Scene->GetResource()->LoadMesh(Mesh_Type::Animation, "PlayerMesh",
	//	TEXT("Player_Default.fbx"));

	m_Scene->GetResource()->LoadMesh(Mesh_Type::Static, "PlayerMesh",
		TEXT("TestTank.FBX"));
}

void CMainSceneMode::CreateMaterial()
{
	
}

void CMainSceneMode::CreateAnimationSequence()
{
}

void CMainSceneMode::CreateParticle()
{
}
