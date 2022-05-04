
#include "RenderStateManager.h"
#include "BlendState.h"
#include "DepthStencilState.h"

CRenderStateManager::CRenderStateManager()
{
}

CRenderStateManager::~CRenderStateManager()
{
}

bool CRenderStateManager::Init()
{
	// D3D11_RENDER_TARGET_BLEND_DESC 구조체 정보를 만들어서 추가한다.
	AddBlendInfo("AlphaBlend");
	CreateBlendState("AlphaBlend", true, false);

	CreateDepthStencilState("DepthDisable", false, D3D11_DEPTH_WRITE_MASK_ZERO);
	
	// 원래는 일반적으로 SrcColor * Alpha + DestColor * (1 - Alpha ) 라는 식으로, 알파 블렌딩 효과를 적용한다.
	// 즉, Src Color 는 현재 픽셀 셰이더에서 넘어온 픽셀 색상
	// Dest Color 는 해당 픽셀 위치에서 Color Buffer 에 저장되어 있는 색상
	// 그런데 조명 누적 버퍼, 즉, 여러개의 조명을 각 픽셀에 적용하여 얻은 색상 정보는
	// 같은 픽셀 위치에 있는 그대로 누적해서 더해가야 한다.
	// 따라서 SrcColor  + DestColor 와 같이. 색상 정보 그대로를 계속 누적해나가는 방식이다.
	AddBlendInfo("LightAcc", true, D3D11_BLEND_ONE, D3D11_BLEND_ONE);
	CreateBlendState("LightAcc", true, false);

	return true;
}

void CRenderStateManager::SetBlendFactor(const std::string& Name,
	float r, float g, float b, float a)
{
	CBlendState* State = (CBlendState*)FindRenderState(Name);

	if (!State)
	{
		State = new CBlendState;

		State->SetName(Name);

		m_mapRenderState.insert(std::make_pair(Name, State));
	}

	State->SetBlendFactor(r, g, b, a);
}

void CRenderStateManager::AddBlendInfo(const std::string& Name, 
	bool BlendEnable, D3D11_BLEND SrcBlend,
	D3D11_BLEND DestBlend, D3D11_BLEND_OP BlendOp, D3D11_BLEND SrcBlendAlpha,
	D3D11_BLEND DestBlendAlpha, D3D11_BLEND_OP BlendOpAlpha, 
	UINT8 RenderTargetWriteMask)
{
	CBlendState* State = (CBlendState*)FindRenderState(Name);

	if (!State)
	{
		State = new CBlendState;

		State->SetName(Name);

		m_mapRenderState.insert(std::make_pair(Name, State));
	}

	State->AddBlendInfo(BlendEnable, SrcBlend, DestBlend, BlendOp,
		SrcBlendAlpha, DestBlendAlpha, BlendOpAlpha, RenderTargetWriteMask);
}

bool CRenderStateManager::CreateBlendState(const std::string& Name, bool AlphaToCoverageEnable,
	bool IndependentBlendEnable)
{
	CBlendState* State = (CBlendState*)FindRenderState(Name);

	if (!State)
		return false;

	if (!State->CreateState(AlphaToCoverageEnable, IndependentBlendEnable))
	{
		SAFE_RELEASE(State);
		return false;
	}

	return true;
}

bool CRenderStateManager::CreateDepthStencilState(const std::string& Name, 
	bool DepthEnable, D3D11_DEPTH_WRITE_MASK DepthWriteMask, 
	D3D11_COMPARISON_FUNC DepthFunc, bool StencilEnable, UINT8 StencilReadMask, 
	UINT8 StencilWriteMask, D3D11_DEPTH_STENCILOP_DESC FrontFace,
	D3D11_DEPTH_STENCILOP_DESC BackFace)
{
	CDepthStencilState* State = (CDepthStencilState*)FindRenderState(Name);

	if (State)
		return false;

	State = new CDepthStencilState;

	if (!State->CreateState(DepthEnable, DepthWriteMask,
		DepthFunc, StencilEnable, StencilReadMask, StencilWriteMask,
		FrontFace, BackFace))
	{
		SAFE_RELEASE(State);
		return false;
	}

	State->SetName(Name);

	m_mapRenderState.insert(std::make_pair(Name, State));

	return true;
}

CRenderState* CRenderStateManager::FindRenderState(const std::string& Name)
{
	auto	iter = m_mapRenderState.find(Name);

	if (iter == m_mapRenderState.end())
		return nullptr;

	return iter->second;
}
