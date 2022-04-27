
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

	// Arm �� Mesh�� �޾��ְ�
	m_MainMesh->AddChild(m_Arm);
	// Arm �� ī�޶� �޾��ش�.
	m_Arm->AddChild(m_Camera);

	// Arm�� ȸ���� ������ �ް� �� ���̴�.
	m_Camera->SetInheritRotX(true);
	m_Camera->SetInheritRotY(true);
	m_Camera->SetInheritRotZ(true);

	m_MainMesh->SetMesh("PlayerMesh");
	m_MainMesh->CreateAnimationInstance<CPlayerAnimation>();
	m_MainMesh->SetRelativeScale(0.05f, 0.05f, 0.05f);

	// ī�޶� �ǵ帱 �ʿ� ����, Arm �� �ǵ帮�� �ȴ�.
	m_Arm->SetOffset(0.f, 1.f, 0.f);

	// Arm �� ȸ����Ű��, ���� �ִ� ī�޶� ȸ���ϰ� �ȴ�.
	m_Arm->SetRelativeRotation(15.f, 0.f, 0.f);

	// �̷��� ó���� TargetDistance �� �ִ���, ȸ������ ������ MainMesh ���� �پ��ִ� ��
	m_Arm->SetTargetDistance(10.f);

	return true;
}

void CPlayer::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	// Y���� �������� ��� ȸ���ϰ� �����Ѵ�.
	// Arm �� Pos �� m_MainMesh ��������, TargetDistance ��ŭ ������ ���¿� ��� ���̰� �� ���̴�.
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
