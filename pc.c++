ID2D1RenderTarget* m_2DTarget;
ID2D1RenderTarget* m_2DTargetWorld;
ID2D1Factory* m_2DFactory;

if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_2DFactory)))
    return false;

IDXGISurface* Surface = nullptr;
m_SwapChain->GetBuffer(0, IID_PRV_ARGS(&Surface));

// 2D 용 렌더 타겟을 만든다.
D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
    D2D1_RENDER_TARGET_TYPE_HARDWARD,
    D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, DXGI_ALPHA_MODE_PREMULTIPLIED)
);

if (FAILED(m_2DFactory->CreateDxgiSufaceRenderTarget(Surface, props, &m_2DTarget)))
    return false;