
#include "RenderTarget.h"
#include "../../Device.h"

CRenderTarget::CRenderTarget()	:
	m_TargetView(nullptr),
	m_TargetTex(nullptr),
	m_PrevTargetView(nullptr),
	m_PrevDepthView(nullptr),
	m_Surface(nullptr),
	m_ClearColor{},
	m_DebugRender(false)
{
}

CRenderTarget::~CRenderTarget()
{
	SAFE_RELEASE(m_Surface);
	SAFE_RELEASE(m_PrevDepthView);
	SAFE_RELEASE(m_PrevTargetView);
	SAFE_RELEASE(m_TargetTex);
	SAFE_RELEASE(m_TargetView);
}

bool CRenderTarget::CreateTarget(const std::string& Name, 
	unsigned int Width, unsigned int Height, DXGI_FORMAT PixelFormat)
{
	SetName(Name);

	// Target�� Texture ����
	D3D11_TEXTURE2D_DESC	Desc = {};

	Desc.Width = Width;
	Desc.Height = Height;
	Desc.ArraySize = 1;
	Desc.MipLevels = 1;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Desc.Format = PixelFormat;
	Desc.Usage = D3D11_USAGE_DEFAULT;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateTexture2D(&Desc, nullptr, &m_TargetTex)))
		return false;

	TextureResourceInfo* Info = new TextureResourceInfo;

	Info->Width = Width;
	Info->Height = Height;
	m_vecTextureInfo.push_back(Info);

	m_TargetTex->QueryInterface(__uuidof(IDXGISurface), (void**)&m_Surface);

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(
		m_TargetTex, nullptr, &Info->SRV)))
		return false;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateRenderTargetView(m_TargetTex,
		nullptr, &m_TargetView)))
		return false;

	return true;
}

void CRenderTarget::ClearTarget()
{
	CDevice::GetInst()->GetContext()->ClearRenderTargetView(m_TargetView, m_ClearColor);
}

void CRenderTarget::SetTarget(ID3D11DepthStencilView* DepthView)
{
	CDevice::GetInst()->GetContext()->OMGetRenderTargets(1, &m_PrevTargetView, &m_PrevDepthView);

	ID3D11DepthStencilView* Depth = m_PrevDepthView;

	if (DepthView)
		Depth = DepthView;

	CDevice::GetInst()->GetContext()->OMSetRenderTargets(1, &m_TargetView, Depth);
}

void CRenderTarget::ResetTarget()
{
	CDevice::GetInst()->GetContext()->OMSetRenderTargets(1, &m_PrevTargetView, m_PrevDepthView);
	SAFE_RELEASE(m_PrevTargetView);
	SAFE_RELEASE(m_PrevDepthView);
}

void CRenderTarget::SetTargetShader()
{
}

void CRenderTarget::ResetTargetShader()
{
}
