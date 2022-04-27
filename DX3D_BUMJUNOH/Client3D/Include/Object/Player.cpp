
#include "Player.h"
#include "PlayerAnimation.h"

CPlayer::CPlayer()
{
	SetTypeID<CPlayer>();
}

CPlayer::CPlayer(const CPlayer& obj)
{
	m_MainMesh = (CAnimationMeshComponent*)FindComponent("Mesh");
	m_Arm = (CArm*)FindComponent("Arm");
	m_Camera = (CCameraComponent*)FindComponent("Camera");
}

CPlayer::~CPlayer()
{
}

bool CPlayer::Init()
{
	m_MainMesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	SetRootComponent(m_MainMesh);

	m_Arm = CreateComponent<CArm>("Arm");
	m_Camera = CreateComponent<CCameraComponent>("Camera");

	// Arm 을 Mesh에 달아주고
	m_MainMesh->AddChild(m_Arm);
	// Arm 에 카메라를 달아준다.
	m_Arm->AddChild(m_Camera);

	// Arm의 회전에 영향을 받게 할 것이다.
	m_Camera->SetInheritRotX(true);
	m_Camera->SetInheritRotY(true);
	m_Camera->SetInheritRotZ(true);

	m_MainMesh->SetMesh("PlayerMesh");
	m_MainMesh->CreateAnimationInstance<CPlayerAnimation>();
	m_MainMesh->SetRelativeScale(0.05f, 0.05f, 0.05f);

	// 카메라를 건드릴 필요 없이, Arm 만 건드리면 된다.
	m_Arm->SetOffset(0.f, 1.f, 0.f);

	// Arm 을 회전시키면, 딸려 있는 카메라도 회전하게 된다.
	m_Arm->SetRelativeRotation(15.f, 0.f, 0.f);

	// 이렇게 처음에 TargetDistance 를 주더라도, 회전축은 여전히 MainMesh 측에 붙어있는 것
	m_Arm->SetTargetDistance(10.f);

	return true;
}

void CPlayer::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	// Y축을 기준으로 계속 회전하게 세팅한다.
	// Arm 의 Pos 는 m_MainMesh 기준으로, TargetDistance 만큼 떨어진 상태에 계속 놓이게 될 것이다.
	m_Arm->AddRelativeRotationY(90.f * DeltaTime);
}

void CPlayer::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CPlayer* CPlayer::Clone()
{
	return new CPlayer(*this);
}

void CPlayer::MoveFront(float DeltaTime)
{
}

void CPlayer::MoveBack(float DeltaTime)
{
}

void CPlayer::RotationYInv(float DeltaTime)
{
}

void CPlayer::RotationY(float DeltaTime)
{
}

void CPlayer::Attack(float DeltaTime)
{
}
