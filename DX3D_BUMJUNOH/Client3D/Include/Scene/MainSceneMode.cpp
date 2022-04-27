
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
	m_Scene->GetResource()->LoadMesh(Mesh_Type::Animation, "PlayerMesh",
		TEXT("Player_Default.msh"));
		// TEXT("Player_Default.msh"));

	// Bone 정보 미리 세팅
	m_Scene->GetResource()->LoadSkeleton("PlayerSkeleton", TEXT("Player_Default.bne"),
		MESH_PATH);

	// Mesh 에 Skeleton 정보를 세팅한다
	m_Scene->GetResource()->SetMeshSkeleton("PlayerMesh", "PlayerSkeleton");

	// Animation 정보를 세팅한다.
	m_Scene->GetResource()->LoadAnimationSequence(true, "PlayerIdle", 
		TEXT("Player_Default.sqc"),MESH_PATH);

	// m_Scene->GetResource()->LoadMesh(Mesh_Type::Static, "PlayerMesh",
	//	TEXT("Hunter.FBX"));

	
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
