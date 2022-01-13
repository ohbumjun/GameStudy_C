ID2D1RenderTarget* m_2DTarget;
ID2D1RenderTarget* m_2DTargetWorld;
ID2D1Factory* m_2DFactory;

if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_2DFactory)))
    return false;

IDXGISurface* BackSurface = nullptr;
m_SwapChain->GetBuffer(0, IID_PRV_ARGS(&BackSurface));


// 2D용 렌더 타겟을 만들어준다.
D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
    D2D1_RENDER_TARGET_TYPE_HARDWARE,
    D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)
);

if (FAILED(m_2DFactory->CreateSxgiSurfaceRenderTarget(BackSurface, props, &m_2DTarget)))
    return false;

SAFE_RELEASE(BackSurface);