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
    if (device_)            device_->Release();
    if (immediate_context_)  immediate_context_->Release();
    if (factory_)         factory_->Release();
    if (swapchain_)         swapchain_->Release();
    if (rtv_)               rtv_->Release();
    if (dsv_)               dsv_->Release();

    device_             = nullptr;
    immediate_context_   = nullptr;
    factory_          = nullptr;
    swapchain_          = nullptr;
    rtv_                = nullptr;
    dsv_                = nullptr;

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
                           D3D11_SDK_VERSION, &device_, &featureLevel, &immediate_context_);
	return hr;
}

HRESULT Device::CreateFactory()
{
    HRESULT hr = S_OK;
    hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory_);

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

    hr = factory_->CreateSwapChain(device_, &sd, &swapchain_);
	return hr;
}

HRESULT Device::CreateRenderTargetView()
{
    HRESULT hr = S_OK;
    // 4)랜더타켓뷰 생성
    ID3D11Texture2D* backBuffer = nullptr;
    swapchain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    hr = device_->CreateRenderTargetView(backBuffer, NULL, &rtv_);
    backBuffer->Release();
	return hr;
}

HRESULT Device::CreateDepthStencilView()
{
    HRESULT hr;
    DXGI_SWAP_CHAIN_DESC scd;
    swapchain_->GetDesc(&scd);
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
    hr = device_->CreateTexture2D(&td, NULL, &DSTexture);

    D3D11_DEPTH_STENCIL_VIEW_DESC dtvd;
    ZeroMemory(&dtvd, sizeof(dtvd));
    dtvd.Format         = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dtvd.ViewDimension  = D3D11_DSV_DIMENSION_TEXTURE2D;
    hr = device_->CreateDepthStencilView(DSTexture, &dtvd, &dsv_);
    DSTexture->Release();

    return hr;
}

void Device::CreateViewPort()
{
    viewport_.Width     = g_rectClient.right;
    viewport_.Height    = g_rectClient.bottom;
    viewport_.TopLeftX  = 0;
    viewport_.TopLeftY  = 0;
    viewport_.MinDepth  = 0.0f;
    viewport_.MaxDepth  = 1.0f;
    immediate_context_->RSSetViewports(1, &viewport_);
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
    if (device_ == nullptr) {
        return S_OK;
    }
    g_rectClient.right = width;
    g_rectClient.bottom = height;

    // unset and release current render target
    if (FAILED(hr = DeleteDXResource())) return hr;
    immediate_context_->OMSetRenderTargets(0, nullptr, NULL);
    rtv_->Release();
    rtv_ = nullptr;
    dsv_->Release();
    dsv_ = nullptr;

    // resize back buffer size
    DXGI_SWAP_CHAIN_DESC CurrentSD;
    swapchain_->GetDesc(&CurrentSD);
    hr = swapchain_->ResizeBuffers(CurrentSD.BufferCount, width, height, CurrentSD.BufferDesc.Format, 0);

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
