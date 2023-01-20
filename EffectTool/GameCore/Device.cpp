#include "Device.h"

bool Device::Init()
{
    if (FAILED(CreateDevice()))             return false;
    if (FAILED(CreateFactory()))            return false;
    if (FAILED(CreateSwapChain()))          return false;
    if (FAILED(CreateRenderTargetView()))   return false;
    if (FAILED(CreateDepthStencilView()))   return false;

    CreateViewPort();
	return true;
}

bool Device::Release()
{
    if (_device)            _device->Release();
    if (_immediateContext)  _immediateContext->Release();
    if (_giFactory)         _giFactory->Release();
    if (_swapChain)         _swapChain->Release();
    if (_rtv)               _rtv->Release();
    if (_dsv)               _dsv->Release();

    _device             = nullptr;
    _immediateContext   = nullptr;
    _giFactory          = nullptr;
    _swapChain          = nullptr;
    _rtv                = nullptr;
    _dsv                = nullptr;

	return true;
}

HRESULT Device::CreateDevice()
{
    HRESULT hr = S_OK;
    UINT CreateDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
    CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif //LiveObject 체크

    D3D_FEATURE_LEVEL featureLevel;
    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
    hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, CreateDeviceFlags, featureLevels, 1,
                           D3D11_SDK_VERSION, &_device, &featureLevel, &_immediateContext);
	return hr;
}

HRESULT Device::CreateFactory()
{
    HRESULT hr = S_OK;
    hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&_giFactory);

	return hr;
}

HRESULT Device::CreateSwapChain()
{
    HRESULT hr = S_OK;
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));

    sd.BufferCount                          = 1; //후면 버퍼 1개 생성
    sd.BufferDesc.Width                     = g_rectClient.right;
    sd.BufferDesc.Height                    = g_rectClient.bottom;
    sd.BufferDesc.Format                    = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator     = 60;
    sd.BufferDesc.RefreshRate.Denominator   = 1;
    sd.BufferUsage                          = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
    sd.OutputWindow                         = g_hWnd;
    sd.SampleDesc.Count                     = 1;
    sd.SampleDesc.Quality                   = 0;
    sd.Windowed                             = true; 
    sd.Flags                                = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    hr = _giFactory->CreateSwapChain(_device, &sd, &_swapChain);
	return hr;
}

HRESULT Device::CreateRenderTargetView()
{
    HRESULT hr = S_OK;
    // 4)랜더타켓뷰 생성
    ID3D11Texture2D* backBuffer = nullptr;
    _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    hr = _device->CreateRenderTargetView(backBuffer, NULL, &_rtv);
    backBuffer->Release();
	return hr;
}

HRESULT Device::CreateDepthStencilView()
{
    HRESULT hr;
    DXGI_SWAP_CHAIN_DESC scd;
    _swapChain->GetDesc(&scd);
    ID3D11Texture2D* DSTexture;
    D3D11_TEXTURE2D_DESC td;
    ZeroMemory(&td, sizeof(td));

    td.Width                = scd.BufferDesc.Width;
    td.Height               = scd.BufferDesc.Height;
    td.MipLevels            = 1;
    td.ArraySize            = 1;
    td.Format               = DXGI_FORMAT_R24G8_TYPELESS;
    td.SampleDesc.Count     = 1;
    td.Usage                = D3D11_USAGE_DEFAULT;
    td.CPUAccessFlags       = 0;
    td.MiscFlags            = 0;
    td.BindFlags            = D3D11_BIND_DEPTH_STENCIL;
    hr = _device->CreateTexture2D(&td, NULL, &DSTexture);

    D3D11_DEPTH_STENCIL_VIEW_DESC dtvd;
    ZeroMemory(&dtvd, sizeof(dtvd));
    dtvd.Format         = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dtvd.ViewDimension  = D3D11_DSV_DIMENSION_TEXTURE2D;
    hr = _device->CreateDepthStencilView(DSTexture, &dtvd, &_dsv);
    DSTexture->Release();

    return hr;
}

void Device::CreateViewPort()
{
    _viewport.Width     = g_rectClient.right;
    _viewport.Height    = g_rectClient.bottom;
    _viewport.TopLeftX  = 0;
    _viewport.TopLeftY  = 0;
    _viewport.MinDepth  = 0.0f;
    _viewport.MaxDepth  = 1.0f;
    _immediateContext->RSSetViewports(1, &_viewport);
}

HRESULT Device::CreateDXResource()
{
    return S_OK;
}

HRESULT Device::DeleteDXResource()
{
    return S_OK;
}

HRESULT Device::ResizeWindow(UINT width, UINT height)
{
    HRESULT hr;
    if (_device == nullptr) {
        return S_OK;
    }
    g_rectClient.right = width;
    g_rectClient.bottom = height;

    // unset and release current render target
    if (FAILED(hr = DeleteDXResource())) return hr;
    _immediateContext->OMSetRenderTargets(0, nullptr, NULL);
    _rtv->Release();
    _rtv = nullptr;
    _dsv->Release();
    _dsv = nullptr;

    // resize back buffer size
    DXGI_SWAP_CHAIN_DESC CurrentSD;
    _swapChain->GetDesc(&CurrentSD);
    hr = _swapChain->ResizeBuffers(CurrentSD.BufferCount, width, height, CurrentSD.BufferDesc.Format, 0);

    // recreate render target view
    if (FAILED(CreateRenderTargetView())) {
        return S_FALSE;
    }
    if (FAILED(CreateDepthStencilView())) {
        return S_FALSE;
    }

    CreateViewPort();
    CreateDXResource();
    return hr;
}
