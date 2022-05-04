
#include "LightManager.h"
#include "Scene.h"
#include "../GameObject/GameObject.h"
#include "../Device.h"

CLightManager::CLightManager()
{
}

CLightManager::~CLightManager()
{
}

void CLightManager::AddLight(CLightComponent* Light)
{
	m_LightList.push_back(Light);
}

void CLightManager::DeleteLight(CLightComponent* Light)
{
	auto	iter = m_LightList.begin();
	auto	iterEnd = m_LightList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == Light)
		{
			m_LightList.erase(iter);
			return;
		}
	}
}

void CLightManager::DeleteLight(const std::string& Name)
{
	auto	iter = m_LightList.begin();
	auto	iterEnd = m_LightList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
		{
			m_LightList.erase(iter);
			return;
		}
	}
}

void CLightManager::Start()
{
}

void CLightManager::Init()
{
	m_GlobalLight = m_Scene->CreateGameObject<CGameObject>("GlobalLight");

	m_GlobalLightComponent = m_GlobalLight->CreateComponent<CLightComponent>("Light");

	m_GlobalLightComponent->SetRelativeRotation(45.f, 90.f, 0.f);

	// 1) 전역광
	m_GlobalLightComponent->SetLightType(Light_Type::Dir);
	 
	// 2) 점광
	// m_GlobalLightComponent->SetRelativePos(0.f, 1.f, -2.f);
	// m_GlobalLightComponent->SetLightType(Light_Type::Point);
	// m_GlobalLightComponent->SetAtt3(0.02f);
	// m_GlobalLightComponent->SetDistance(100.f);

	// 3) Spot Light
	// m_GlobalLightComponent->SetRelativePos(0.f, 4.f, -5.f);
	// m_GlobalLightComponent->SetLightType(Light_Type::Spot);
	// m_GlobalLightComponent->SetDistance(100.f);
	// m_GlobalLightComponent->SetAtt3(0.0001f);
}

void CLightManager::Update(float DeltaTime)
{
	auto	iter = m_LightList.begin();
	auto	iterEnd = m_LightList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_LightList.erase(iter);
			iterEnd = m_LightList.end();

			continue;
		}

		++iter;
	}
}

void CLightManager::PostUpdate(float DeltaTime)
{
}

void CLightManager::SetShader()
{
	auto	iter = m_LightList.begin();
	auto	iterEnd = m_LightList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsEnable())
			continue;

		(*iter)->SetShader();
	}
}

void CLightManager::Destroy()
{
	m_LightList.clear();
	m_GlobalLightComponent = nullptr;
	m_GlobalLight = nullptr;
}

void CLightManager::Render()
{
	// 이제 여기서 조명 누적 버퍼를 만들 것이다.
	// 화면 꽉 찬 버퍼를 만들어서, 각 픽셀마다 조명 처리를 해줘야 한다.
	// 각 픽셀마다 물체가 출력되어 있다면
	// GBuffer 에 값이 있을 것이다.
	// 값이 안들어가 있는 것은 Clip 할 것이다.
	// 값이 있는 것은, 조명 계산을 수행할 것이다.
	// ex) 누적 -> 방향성 1개, 점 조명 2개 -> 한 픽셀을 모두 비춘다면, 그 부분은 매우 밝게 보여야 한다.

	// 이를 위해 Full Screen 사각형을 출력할 것이다.
	// Null Buffer 라는 시스템을 사용할 것이다.
	// Vertex Buffer, Idx Buffer  를 Null 로 하여 출력하면, DX11 은 화면을 꽉 채운
	// 사각형을 출력한다.
	// 이를 Null Buffer 라고 한다.

	CShader* Shader = m_Scene->GetResource()->FindShader("LightAccShader");

	// Light.fx 셰이더 파일을 컴파일해서 얻는 Shader 객체를 넘겨준다.
	Shader->SetShader();

	auto iter = m_LightList.begin();
	auto iterEnd = m_LightList.end();

	bool SendTransform = false;

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsEnable())
			continue;

		// Transform 정보는 딱 한번만 넘겨주면 된다.
		// View 행렬로 돌아가게 하기 위해, 투영 행렬의 역행렬을 곱해줘야 하는데
		// 이것은 Light Component가 모두 동일한 값을 지니고 있기 때문이다.
		if (!SendTransform)
		{
			(*iter)->GetTransform()->SetTransform();
			SendTransform = true;
		}

		// 각각의 Light Component가 들고 있는 Light 상수 버퍼 정보를 넘겨준다.
		(*iter)->SetShader();

		// Null Buffer 에 해당하는 Mesh 정보를 세팅해서 넘겨준다.
		UINT Offset = 0;
		CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 0, nullptr, nullptr, &Offset);
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

		// Idx 버퍼 사용 정보 없이, 4개의 정점을 그려낸다.
		CDevice::GetInst()->GetContext()->Draw(4, 0);
	}
}
