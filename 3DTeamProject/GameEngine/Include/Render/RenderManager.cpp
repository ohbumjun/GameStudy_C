
#include "RenderManager.h"
#include "../GameObject/GameObject.h"
#include "../Component/SceneComponent.h"
#include "RenderStateManager.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Texture/RenderTarget.h"
#include "RenderState.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/Viewport.h"
#include "../Engine.h"
#include "../Device.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager()	:
	m_RenderStateManager(nullptr),
	m_Standard2DCBuffer(nullptr)
{
}

CRenderManager::~CRenderManager()
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}

	m_RenderLayerList.clear();

	SAFE_DELETE(m_Standard2DCBuffer);
	SAFE_DELETE(m_RenderStateManager);
}

void CRenderManager::AddRenderList(CSceneComponent* Component)
{
	RenderLayer* Layer = nullptr;

	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == Component->GetLayerName())
		{
			Layer = *iter;
			break;
		}
	}

	if (!Layer)
		return;

	if (Layer->RenderCount == (int)Layer->RenderList.size())
		Layer->RenderList.resize(Layer->RenderCount * 2);

	Layer->RenderList[Layer->RenderCount] = Component;
	++Layer->RenderCount;
}

void CRenderManager::CreateLayer(const std::string& Name, int Priority)
{
	RenderLayer* Layer = new RenderLayer;
	Layer->Name = Name;
	Layer->LayerPriority = Priority;

	m_RenderLayerList.push_back(Layer);

	sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), CRenderManager::Sortlayer);
}

void CRenderManager::SetLayerPriority(const std::string& Name, int Priority)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == Name)
		{
			(*iter)->LayerPriority = Priority;
			break;
		}
	}

	sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), CRenderManager::Sortlayer);
}

bool CRenderManager::Init()
{
	m_RenderStateManager = new CRenderStateManager;

	m_RenderStateManager->Init();

	m_Standard2DCBuffer = new CStandard2DConstantBuffer;

	m_Standard2DCBuffer->Init();

	// �⺻ ���̾� ����
	RenderLayer* Layer = new RenderLayer;
	Layer->Name = "Back";
	Layer->LayerPriority = 0;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "Default";
	Layer->LayerPriority = 1;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "Particle";
	Layer->LayerPriority = 2;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "ScreenWidgetComponent";
	Layer->LayerPriority = 3;

	m_RenderLayerList.push_back(Layer);

	m_DepthDisable = m_RenderStateManager->FindRenderState("DepthDisable");
	m_AlphaBlend = m_RenderStateManager->FindRenderState("AlphaBlend");
	m_LightAccBlend = m_RenderStateManager->FindRenderState("LightAcc");

	// ���۵� �������� Ÿ�� ����
	Resolution RS = CDevice::GetInst()->GetResolution();

	// ����, Diffuse (����), Normal, Pos --> 4���� �ʿ��غ��δ�.
	// ������ ��� -> �� 4byte ũ�� -> r,g,b,a ���� ������ �����Ѵ�.
	// ��, �ϳ���, 1byte -> 8bit -> 0 ~ 255 ���ڸ� ǥ���� �� �ִ�.
	if (!CResourceManager::GetInst()->CreateTarget("Diffuse",
		RS.Width, RS.Height, DXGI_FORMAT_R8G8B8A8_UNORM))
		return false;

	// �ʿ��� ���, Buffer ���� �߰��Ͽ� ������ ���̴�.
	// ���ĺ��ʹ�, �� ä�δ� 4byte �� ����ϴ� ũ���� Buffer �� 
	if (!CResourceManager::GetInst()->CreateTarget("GBuffer1",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("GBuffer2",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("GBuffer3",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	// �Ʒ��� ���� 4���� ���۸� m_vecGBuffer ��� RenderTarget �迭�� ������ش�.
	// ���� ���� ���� Ÿ�ٵ��� ��Ƽ� ó���ϴ� �ý����� �۾��� ���̴�.
	CRenderTarget* GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("Diffuse");
	GBufferTarget->SetPos(Vector3(0.f, 0.f, 0.f));
	GBufferTarget->SetScale(Vector3(150.f, 150.f, 1.f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer1");
	GBufferTarget->SetPos(Vector3(0.f, 150.f, 0.f));
	GBufferTarget->SetScale(Vector3(150.f, 150.f, 1.f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer2");
	GBufferTarget->SetPos(Vector3(0.f, 300.f, 0.f));
	GBufferTarget->SetScale(Vector3(150.f, 150.f, 1.f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer3");
	GBufferTarget->SetPos(Vector3(0.f, 450.f, 0.f));
	GBufferTarget->SetScale(Vector3(150.f, 150.f, 1.f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);

	// ���� ó���� ���� 4���� ���� ���� ������ �ʿ��ϴ�
	if (!CResourceManager::GetInst()->CreateTarget("LightDif", // Dif + Amb
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("LightSpc",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("LightEmv",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	CRenderTarget* LightTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("LightDif");
	LightTarget->SetPos(Vector3(150.f, 0.f, 0.f));
	LightTarget->SetScale(Vector3(150.f, 150.f, 1.f));
	LightTarget->SetDebugRender(true);
	m_vecLightBuffer.push_back(LightTarget);

	LightTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("LightSpc");
	LightTarget->SetPos(Vector3(150.f, 150.f, 0.f));
	LightTarget->SetScale(Vector3(150.f, 150.f, 1.f));
	LightTarget->SetDebugRender(true);
	m_vecLightBuffer.push_back(LightTarget);

	LightTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("LightEmv");
	LightTarget->SetPos(Vector3(150.f, 300.f, 0.f));
	LightTarget->SetScale(Vector3(150.f, 150.f, 1.f));
	LightTarget->SetDebugRender(true);
	m_vecLightBuffer.push_back(LightTarget);

	// ���� ������� ���� Render Target �����
	if (!CResourceManager::GetInst()->CreateTarget("FinalScreen",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	CRenderTarget* FinalTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("FinalScreen");
	FinalTarget->SetPos(Vector3(300.f, 0.f, 0.f));
	FinalTarget->SetScale(Vector3(150.f, 150.f, 1.f));
	FinalTarget->SetDebugRender(true);

	// m_LightBlendShader;
	// m_LightBlendRenderShader;

	return true;
}

void CRenderManager::Render()
{
	{
		auto	iter = m_RenderLayerList.begin();
		auto	iterEnd = m_RenderLayerList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->RenderCount = 0;
		}
	}

	{
		auto	iter = m_ObjectList->begin();
		auto	iterEnd = m_ObjectList->end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->PrevRender();
		}	
	}

	RenderGBuffer();

	RenderLightAcc();

	/*
	// ���� ������ Shader�� �Ѱ��ش�.
	CSceneManager::GetInst()->GetScene()->GetLightManager()->SetShader();

	{
		auto	iter = m_RenderLayerList.begin();
		auto	iterEnd = m_RenderLayerList.end();

		for (; iter != iterEnd; ++iter)
		{
			for (int i = 0; i < (*iter)->RenderCount; ++i)
			{
				(*iter)->RenderList[i]->Render();
			}
		}
	}
	*/

	{
		auto	iter = m_RenderLayerList.begin();
		auto	iterEnd = m_RenderLayerList.end();

		for (; iter != iterEnd; ++iter)
		{
			for (int i = 0; i < (*iter)->RenderCount; ++i)
			{
				(*iter)->RenderList[i]->PostRender();
			}
		}
	}

	// Widget ���
	m_DepthDisable->SetState();

	m_AlphaBlend->SetState();

	// UI Rendering
	CSceneManager::GetInst()->GetScene()->GetViewport()->Render();

	// ������� ���� ���� Ÿ���� ����Ѵ�.
	CResourceManager::GetInst()->RenderTarget();

	// ���콺 ���
	CWidgetWindow* MouseWidget = CEngine::GetInst()->GetMouseWidget();

	if (MouseWidget)
		MouseWidget->Render();

	m_AlphaBlend->ResetState();

	m_DepthDisable->ResetState();
}

void CRenderManager::RenderGBuffer()
{
	// ���� ����۷� ���� Ÿ���� �����Ǿ� �ִ�.
	// �̸� GBuffer ���� Ÿ������ ��ü�ؾ� �Ѵ�.
	std::vector<ID3D11RenderTargetView*> vecTarget;
	std::vector<ID3D11RenderTargetView*> vecPrevTarget;

	ID3D11DepthStencilView* PrevDepthTarget = nullptr;

	size_t GBufferSize = m_vecGBuffer.size();

	vecPrevTarget.resize(GBufferSize);
	vecTarget.reserve(GBufferSize);

	for (size_t i = 0; i < GBufferSize; ++i)
	{
		// ������ �� GBuffer ���� �ѹ� �����ְ�, �ٽ� Render �Ѵ�.
		m_vecGBuffer[i]->ClearTarget();
		vecTarget.push_back(m_vecGBuffer[i]->GetTargetView());
	}

	// ���� �����Ǿ� �ִ� ����Ÿ�ٰ� ����Ÿ���� ���´�.
	// �Ƹ�. ������ �׸��� Back Buffer �ϳ��ΰǰ�..?
	CDevice::GetInst()->GetContext()->OMGetRenderTargets((unsigned int)GBufferSize,
		&vecPrevTarget[0], &PrevDepthTarget);

	// GBuffer �� �ش��ϴ� Render Target ��� ��ü���ش�.
	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)GBufferSize,
		&vecTarget[0], PrevDepthTarget);

	// ���� Back, Default �� �ش��ϴ� Render �� �׷��� ���̴�.
	// ��, ���� ó���� ���ֱ� ���� �༮�鸸 �׷��� �Ѵ�.
	for (size_t i = 0; i <= 1; ++i)
	{
		for (int j = 0; j < m_RenderLayerList[i]->RenderCount; ++j)
		{
			m_RenderLayerList[i]->RenderList[j]->Render();
		}
	}

	// �ٽ� �� ó�� �ϳ��� Back Buffer ������ �������ش�. (������� �ǵ����ֱ� )
	// Depth Stencile ���۴� ������� �ʱ� ������, PrevDepthTarget �� ������ ����Ѵ�.
	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)GBufferSize,
		&vecPrevTarget[0], PrevDepthTarget);

	SAFE_RELEASE(PrevDepthTarget);

	for (size_t i = 0; i < GBufferSize; ++i)
	{
		SAFE_RELEASE(vecPrevTarget[i]);
	}
}

void CRenderManager::RenderLightAcc()
{
	std::vector<ID3D11RenderTargetView*> vecTarget;
	std::vector<ID3D11RenderTargetView*> vecPrevTarget;
	ID3D11DepthStencilView* PrevDepthTarget = nullptr;

	size_t LightBufferSize = m_vecLightBuffer.size();
	vecPrevTarget.resize(LightBufferSize);

	for (size_t i = 0; i < LightBufferSize; ++i)
	{
		m_vecLightBuffer[i]->ClearTarget();
		vecTarget.push_back(m_vecLightBuffer[i]->GetTargetView());
	}

	// ���� �����Ǿ� �ִ� ����Ÿ�ٰ� ����Ÿ���� ���´�.
	CDevice::GetInst()->GetContext()->OMGetRenderTargets((unsigned int)LightBufferSize,
		&vecPrevTarget[0], &PrevDepthTarget);

	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)LightBufferSize,
		&vecTarget[0], PrevDepthTarget);

	// Light Acc Blend �� �����Ͽ� �׷��� ���̴�.
	m_LightAccBlend->SetState();

	// ���� ���۸� ��Ȱ��ȭ�ؾ� �Ѵ�.
	// ���� ���ۿ� ���̰��� �����صθ�
	// ���� ���� ���۸� ���� ��ϵ� ������ ����
	// ���� �׷��� Particle ���� �������� ��� ������ �Ⱥ��ϰ� �Ǳ� �����̴�.
	m_DepthDisable->SetState();

	// ���� ���� ���� ���۳��� Texture �������� Shader ���� �Ѱ��ش�.
	m_vecGBuffer[0]->SetTargetShader(14);
	m_vecGBuffer[1]->SetTargetShader(15);
	m_vecGBuffer[2]->SetTargetShader(16);
	m_vecGBuffer[3]->SetTargetShader(17);

	// ����, Light �� �׷�����.
	CSceneManager::GetInst()->GetScene()->GetLightManager()->Render();

	// Reset
	m_vecGBuffer[0]->ResetTargetShader(14);
	m_vecGBuffer[1]->ResetTargetShader(15);
	m_vecGBuffer[2]->ResetTargetShader(16);
	m_vecGBuffer[3]->ResetTargetShader(17);

	m_DepthDisable->ResetState();

	m_LightAccBlend->ResetState();

	// ������ Render Target ���� �ǵ�����.
	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)LightBufferSize,
		&vecPrevTarget[0], PrevDepthTarget);

	SAFE_RELEASE(PrevDepthTarget);
	for (size_t i = 0; i < LightBufferSize; ++i)
	{
		SAFE_RELEASE(vecPrevTarget[i]);
	}
}

void CRenderManager::SetBlendFactor(const std::string& Name, float r, float g,
	float b, float a)
{
	m_RenderStateManager->SetBlendFactor(Name, r, g, b, a);
}

void CRenderManager::AddBlendInfo(const std::string& Name, bool BlendEnable,
	D3D11_BLEND SrcBlend, D3D11_BLEND DestBlend, D3D11_BLEND_OP BlendOp, 
	D3D11_BLEND SrcBlendAlpha, D3D11_BLEND DestBlendAlpha, 
	D3D11_BLEND_OP BlendOpAlpha, UINT8 RenderTargetWriteMask)
{
	m_RenderStateManager->AddBlendInfo(Name, BlendEnable, SrcBlend, DestBlend,
		BlendOp, SrcBlendAlpha, DestBlendAlpha, BlendOpAlpha, RenderTargetWriteMask);
}

bool CRenderManager::CreateBlendState(const std::string& Name, 
	bool AlphaToCoverageEnable, bool IndependentBlendEnable)
{
	return m_RenderStateManager->CreateBlendState(Name, AlphaToCoverageEnable, IndependentBlendEnable);
}

CRenderState* CRenderManager::FindRenderState(const std::string& Name)
{
	return m_RenderStateManager->FindRenderState(Name);
}

bool CRenderManager::Sortlayer(RenderLayer* Src, RenderLayer* Dest)
{
	return Src->LayerPriority < Dest->LayerPriority;
}
