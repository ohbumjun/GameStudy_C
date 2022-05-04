
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

	// 1) ������
	m_GlobalLightComponent->SetLightType(Light_Type::Dir);
	 
	// 2) ����
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
	// ���� ���⼭ ���� ���� ���۸� ���� ���̴�.
	// ȭ�� �� �� ���۸� ����, �� �ȼ����� ���� ó���� ����� �Ѵ�.
	// �� �ȼ����� ��ü�� ��µǾ� �ִٸ�
	// GBuffer �� ���� ���� ���̴�.
	// ���� �ȵ� �ִ� ���� Clip �� ���̴�.
	// ���� �ִ� ����, ���� ����� ������ ���̴�.
	// ex) ���� -> ���⼺ 1��, �� ���� 2�� -> �� �ȼ��� ��� ����ٸ�, �� �κ��� �ſ� ��� ������ �Ѵ�.

	// �̸� ���� Full Screen �簢���� ����� ���̴�.
	// Null Buffer ��� �ý����� ����� ���̴�.
	// Vertex Buffer, Idx Buffer  �� Null �� �Ͽ� ����ϸ�, DX11 �� ȭ���� �� ä��
	// �簢���� ����Ѵ�.
	// �̸� Null Buffer ��� �Ѵ�.

	CShader* Shader = m_Scene->GetResource()->FindShader("LightAccShader");

	// Light.fx ���̴� ������ �������ؼ� ��� Shader ��ü�� �Ѱ��ش�.
	Shader->SetShader();

	auto iter = m_LightList.begin();
	auto iterEnd = m_LightList.end();

	bool SendTransform = false;

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsEnable())
			continue;

		// Transform ������ �� �ѹ��� �Ѱ��ָ� �ȴ�.
		// View ��ķ� ���ư��� �ϱ� ����, ���� ����� ������� ������� �ϴµ�
		// �̰��� Light Component�� ��� ������ ���� ���ϰ� �ֱ� �����̴�.
		if (!SendTransform)
		{
			(*iter)->GetTransform()->SetTransform();
			SendTransform = true;
		}

		// ������ Light Component�� ��� �ִ� Light ��� ���� ������ �Ѱ��ش�.
		(*iter)->SetShader();

		// Null Buffer �� �ش��ϴ� Mesh ������ �����ؼ� �Ѱ��ش�.
		UINT Offset = 0;
		CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 0, nullptr, nullptr, &Offset);
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

		// Idx ���� ��� ���� ����, 4���� ������ �׷�����.
		CDevice::GetInst()->GetContext()->Draw(4, 0);
	}
}
