
#include "RenderStateManager.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"

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

	// SkyBox -> Front Face Culling
	CreateRasterizerState("FrontFaceCulling", D3D11_FILL_SOLID, D3D11_CULL_FRONT);

	// 1) Depth Write Mask 
	// D3D11_DEPTH_WRITE_MASK_ZERO -> 깊이 버퍼 쓰기.가 방지되지만, 깊이 판정 수행
	// D3D11_DEPTH_WRITE_MASK_ALL -> 깊이 버퍼 쓰기 활성화 -> 깊이 판정, 스탠실 판정을 모두 통과한
	//			픽셀의 깊이가 깊이 버퍼에 새로 기록된다.
	// 아래의 경우, 둘 중에서 아무것이나 상관없다.

	// 2) DepthFuc -> 깊이 판정에 쓰이는 비교 함수 
	// 통상적인 깊이 판정의 경우, 후면 버퍼에 이미 기록되어 있는 픽셀 깊이보다
	// 현재 픽셀 깊이가 더 작은 경우에만 ! 갱신을 하고자 한다면 D3D11_COMPARISON_LESS
	// 즉, 크거나 같으면 나오지 않는다.
	// True 를 리턴한다면, 해당 픽셀은 후면 버퍼에 기록된다. (깊이 판정도 통과했따고 가정할 때)
	// False 리면, 해당 픽셀은 후면 버퍼에 기록되지 않고, 깊이 버퍼에도 기록되지 않는다.
	
	// Sky 의 경우, 항상 제일 멀리 있어야 한다.
	// 그러면, Z가 기본적으로 Depth StencilState 에서 깊이가 1보다 작아야 한다.
	// 그런데 우리는 D3D11_COMPARISON_LESS_EQUAL 로 바꿀 것이다
	// 작거나 "같아도" 렌더링 될 수 있도록 하는 것이다.
	// 즉, 같은 깊이라면, 나중에 그리는 삼각형이 더 앞에 왔으면 좋겠다.
	
	// Sky 의 경우, NDC 좌표 변환 이후 Z 값이 최종 1이 되도록 렌더링할 것이다.
	// 1보다 더 큰 Z 값을 가진 물체는 아예 나오지 않게 될 것이다
	// 따라서 SkySphere 를 뚫고 나가서도 보이는 일은 없다
	// 즉, Sky가 가장 멀리 보이게 될 것이다.
	CreateDepthStencilState("SkyDepth", true, D3D11_DEPTH_WRITE_MASK_ZERO,
		D3D11_COMPARISON_LESS_EQUAL);

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

bool CRenderStateManager::CreateRasterizerState(
	const std::string& Name, D3D11_FILL_MODE FillMode,
	D3D11_CULL_MODE CullMode, BOOL FrontCounterClockwise,
	INT DepthBias, FLOAT DepthBiasClamp,
	FLOAT SlopeScaledDepthBias, BOOL DepthClipEnable,
	BOOL ScissorEnable, BOOL MultisampleEnable,
	BOOL AntialiasedLineEnable)
{
	CRasterizerState* State = (CRasterizerState*)FindRenderState(Name);

	if (State)
		return false;

	State = new CRasterizerState;

	if (!State->CreateState(FillMode, CullMode,
		FrontCounterClockwise, DepthBias, DepthBiasClamp,
		SlopeScaledDepthBias,
		DepthClipEnable, ScissorEnable, MultisampleEnable,
		AntialiasedLineEnable))
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
