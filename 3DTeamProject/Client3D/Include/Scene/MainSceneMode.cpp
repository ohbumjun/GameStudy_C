
#include "MainSceneMode.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/Viewport.h"
#include "../Object/Player.h"
#include "GameObject/LightObj.h"

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

	CLightObj* Light = m_Scene->CreateGameObject<CLightObj>("Light1");

	((CLightComponent*)Light->GetRootComponent())->SetRelativePos(-3.f, 5.f, 0.f);
	((CLightComponent*)Light->GetRootComponent())->SetLightType(Light_Type::Point);
	((CLightComponent*)Light->GetRootComponent())->SetDistance(10.f);
	((CLightComponent*)Light->GetRootComponent())->SetAtt3(0.02f);
	((CLightComponent*)Light->GetRootComponent())->SetColor(Vector4(1.f, 1.f, 1.f, 1.f));
		
	CLightObj* Light2 = m_Scene->CreateGameObject<CLightObj>("Light2");

	((CLightComponent*)Light2->GetRootComponent())->SetRelativePos(3.f, 5.f, 0.f);
	((CLightComponent*)Light2->GetRootComponent())->SetLightType(Light_Type::Point);
	((CLightComponent*)Light2->GetRootComponent())->SetDistance(10.f);
	((CLightComponent*)Light2->GetRootComponent())->SetAtt3(0.02f);
	((CLightComponent*)Light2->GetRootComponent())->SetColor(Vector4(1.f, 1.f, 0.f, 1.f));
	
	return true;
}

void CMainSceneMode::LoadMesh()
{
	/*
	m_Scene->GetResource()->LoadMesh(Mesh_Type::Animation, "PlayerMesh",
		TEXT("Jockey_crouchWalkE_NormalMap.msh"));

	m_Scene->GetResource()->LoadSkeleton("PlayerSkeleton",
		TEXT("Jockey_crouchWalkE_NormalMap.bne"), MESH_PATH);

	m_Scene->GetResource()->SetMeshSkeleton("PlayerMesh", "PlayerSkeleton");

	m_Scene->GetResource()->LoadAnimationSequence(true, "PlayerIdle",
		TEXT("Jockey_crouchWalkE_NormalMap.sqc"), MESH_PATH);
		*/

	/*
	m_Scene->GetResource()->LoadMesh(Mesh_Type::Animation, "PlayerMesh",
		TEXT("Player_Default.msh"));

	m_Scene->GetResource()->LoadSkeleton("PlayerSkeleton",
		TEXT("Player_Default.bne"), MESH_PATH);

	m_Scene->GetResource()->SetMeshSkeleton("PlayerMesh", "PlayerSkeleton");

	m_Scene->GetResource()->LoadAnimationSequence(true, "PlayerIdle",
		TEXT("Player_Default.sqc"), MESH_PATH);

	m_Scene->GetResource()->LoadAnimationSequence(false, "PlayerAttack",
		TEXT("PlayerAttack.sqc"), MESH_PATH);

	m_Scene->GetResource()->LoadAnimationSequence(true, "PlayerWalk",
		TEXT("PlayerWalk.sqc"), MESH_PATH);
		*/

	m_Scene->GetResource()->LoadMesh(Mesh_Type::Animation, "PlayerMesh",
		TEXT("vladi_spell4.msh"));

	m_Scene->GetResource()->LoadSkeleton("PlayerSkeleton",
		TEXT("vladi_spell4.bne"), MESH_PATH);

	m_Scene->GetResource()->SetMeshSkeleton("PlayerMesh", "PlayerSkeleton");

	m_Scene->GetResource()->LoadAnimationSequence(true, "PlayerIdle",
		TEXT("vladi_spell4.sqc"), MESH_PATH);

	m_Scene->GetResource()->LoadAnimationSequence(false, "PlayerAttack",
		TEXT("Vladi_attack1.sqc"), MESH_PATH);

	m_Scene->GetResource()->LoadAnimationSequence(true, "PlayerWalk",
		TEXT("vladi_spell4.sqc"), MESH_PATH);
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
