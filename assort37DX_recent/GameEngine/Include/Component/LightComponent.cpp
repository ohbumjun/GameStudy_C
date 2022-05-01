
#include "LightComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/LightManager.h"
#include "../Scene/CameraManager.h"
#include "CameraComponent.h"

CLightComponent::CLightComponent()
{
	SetTypeID<CLightComponent>();
	m_Render = false;

	m_CBuffer = nullptr;
}

CLightComponent::CLightComponent(const CLightComponent& com) :
	CSceneComponent(com)
{
	m_CBuffer = com.m_CBuffer->Clone();
}

CLightComponent::~CLightComponent()
{
	// 자기가 사라질 때 LightManager 에서도 제거해줄 수 있도록 세팅
	m_Scene->GetLightManager()->DeleteLight(this);
	SAFE_DELETE(m_CBuffer);
}

void CLightComponent::Start()
{
	CSceneComponent::Start();

	// Start 함수 안에 들어온다는 의미는
	// 실제 World 에 배치된 조명이라는 의미이다.
	m_Scene->GetLightManager()->AddLight(this);
}

bool CLightComponent::Init()
{
	m_CBuffer = new CLightConstantBuffer;
	
	m_CBuffer->Init();

	return true;
}

void CLightComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CLightComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	CCameraComponent* Camera = m_Scene->GetCameraManager()->GetCurrentCamera();

	// 방향성 조명이 아니라면, 즉, 점 조명, Point 조명은, 조명의 위치.가 필요하다.
	if (m_CBuffer->GetLightType() != Light_Type::Dir)
	{
		Vector3	Pos = GetWorldPos();

		// 뷰 공간으로 변환한다.
		Pos = Pos.TransformCoord(Camera->GetViewMatrix());

		m_CBuffer->SetPos(Pos);

		if (m_CBuffer->GetLightType() == Light_Type::Point)
			SetRelativeScale(m_CBuffer->GetLightDistance(), m_CBuffer->GetLightDistance(), m_CBuffer->GetLightDistance());
	}

	// 점 조명이 아니라면,
	// 방향 벡터 정보가 필요하다.
	if(m_CBuffer->GetLightType() != Light_Type::Point)
	{
		Vector3 Dir = GetWorldAxis(AXIS_Z);

		// 뷰공간으로 변환한다.
		Dir = Dir.TransformNormal(Camera->GetViewMatrix());
		Dir.Normalize();

		m_CBuffer->SetDir(Dir);
	}
}

void CLightComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CLightComponent::Render()
{
	CSceneComponent::Render();
}

void CLightComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CLightComponent* CLightComponent::Clone()
{
	return new CLightComponent(*this);
}

void CLightComponent::Save(FILE* File)
{
	CSceneComponent::Save(File);
}

void CLightComponent::Load(FILE* File)
{
	CSceneComponent::Load(File);
}

void CLightComponent::SetShader()
{
	m_CBuffer->UpdateCBuffer();
}
