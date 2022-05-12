
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
	// D3D11_RENDER_TARGET_BLEND_DESC ����ü ������ ���� �߰��Ѵ�.
	AddBlendInfo("AlphaBlend");
	CreateBlendState("AlphaBlend", true, false);

	CreateDepthStencilState("DepthDisable", false, D3D11_DEPTH_WRITE_MASK_ZERO);
	
	// ������ �Ϲ������� SrcColor * Alpha + DestColor * (1 - Alpha ) ��� ������, ���� ���� ȿ���� �����Ѵ�.
	// ��, Src Color �� ���� �ȼ� ���̴����� �Ѿ�� �ȼ� ����
	// Dest Color �� �ش� �ȼ� ��ġ���� Color Buffer �� ����Ǿ� �ִ� ����
	// �׷��� ���� ���� ����, ��, �������� ������ �� �ȼ��� �����Ͽ� ���� ���� ������
	// ���� �ȼ� ��ġ�� �ִ� �״�� �����ؼ� ���ذ��� �Ѵ�.
	// ���� SrcColor  + DestColor �� ����. ���� ���� �״�θ� ��� �����س����� ����̴�.
	AddBlendInfo("LightAcc", true, D3D11_BLEND_ONE, D3D11_BLEND_ONE);
	CreateBlendState("LightAcc", true, false);

	// SkyBox -> Front Face Culling
	CreateRasterizerState("FrontFaceCulling", D3D11_FILL_SOLID, D3D11_CULL_FRONT);

	// 1) Depth Write Mask 
	// D3D11_DEPTH_WRITE_MASK_ZERO -> ���� ���� ����.�� ����������, ���� ���� ����
	// D3D11_DEPTH_WRITE_MASK_ALL -> ���� ���� ���� Ȱ��ȭ -> ���� ����, ���Ľ� ������ ��� �����
	//			�ȼ��� ���̰� ���� ���ۿ� ���� ��ϵȴ�.
	// �Ʒ��� ���, �� �߿��� �ƹ����̳� �������.

	// 2) DepthFuc -> ���� ������ ���̴� �� �Լ� 
	// ������� ���� ������ ���, �ĸ� ���ۿ� �̹� ��ϵǾ� �ִ� �ȼ� ���̺���
	// ���� �ȼ� ���̰� �� ���� ��쿡�� ! ������ �ϰ��� �Ѵٸ� D3D11_COMPARISON_LESS
	// ��, ũ�ų� ������ ������ �ʴ´�.
	// True �� �����Ѵٸ�, �ش� �ȼ��� �ĸ� ���ۿ� ��ϵȴ�. (���� ������ ����ߵ��� ������ ��)
	// False ����, �ش� �ȼ��� �ĸ� ���ۿ� ��ϵ��� �ʰ�, ���� ���ۿ��� ��ϵ��� �ʴ´�.
	
	// Sky �� ���, �׻� ���� �ָ� �־�� �Ѵ�.
	// �׷���, Z�� �⺻������ Depth StencilState ���� ���̰� 1���� �۾ƾ� �Ѵ�.
	// �׷��� �츮�� D3D11_COMPARISON_LESS_EQUAL �� �ٲ� ���̴�
	// �۰ų� "���Ƶ�" ������ �� �� �ֵ��� �ϴ� ���̴�.
	// ��, ���� ���̶��, ���߿� �׸��� �ﰢ���� �� �տ� ������ ���ڴ�.
	
	// Sky �� ���, NDC ��ǥ ��ȯ ���� Z ���� ���� 1�� �ǵ��� �������� ���̴�.
	// 1���� �� ū Z ���� ���� ��ü�� �ƿ� ������ �ʰ� �� ���̴�
	// ���� SkySphere �� �հ� �������� ���̴� ���� ����
	// ��, Sky�� ���� �ָ� ���̰� �� ���̴�.
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
