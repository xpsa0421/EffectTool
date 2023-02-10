#include "Device.h"

bool Device::Init()
{
    device_				= nullptr;
    device_context_		= nullptr;
    factory_			= nullptr;
    swapchain_			= nullptr;
    rtv_				= nullptr;
    dsv_				= nullptr;

    if (FAILED(CreateDevice()))             return false;
    if (FAILED(CreateFactory()))            return false;
    if (FAILED(CreateSwapChain()))          return false;
    if (FAILED(CreateRenderTargetView()))   return false;
    if (FAILED(CreateDepthStencilView()))   return false;

    CreateViewport();
	return true;
}

bool Device::Release()
{
    device_             = nullptr;
    device_context_     = nullptr;
    factory_            = nullptr;
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
#endif 

    D3D_FEATURE_LEVEL featureLevel;
    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
    hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, CreateDeviceFlags, featureLevels, 1,
                           D3D11_SDK_VERSION, device_.GetAddressOf(), &featureLevel, device_context_.GetAddressOf());
	return hr;
}

HRESULT Device::CreateFactory()
{
    HRESULT hr = S_OK;
    hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)factory_.GetAddressOf());

	return hr;
}

HRESULT Device::CreateSwapChain()
{
    HRESULT hr = S_OK;
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));

    sd.BufferCount                          = 1; 
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

    hr = factory_->CreateSwapChain(device_.Get(), &sd, swapchain_.GetAddressOf());
	return hr;
}

HRESULT Device::CreateRenderTargetView()
{
    HRESULT hr = S_OK;
    ComPtr<ID3D11Texture2D> backBuffer = nullptr;
    swapchain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
    hr = device_->CreateRenderTargetView(backBuffer.Get(), NULL, &rtv_);
    backBuffer = nullptr;

	return hr;
}

HRESULT Device::CreateDepthStencilView()
{
    HRESULT hr;
    DXGI_SWAP_CHAIN_DESC scd;
    swapchain_->GetDesc(&scd);
    ComPtr<ID3D11Texture2D> texture = nullptr;
    D3D11_TEXTURE2D_DESC texture_desc;
    ZeroMemory(&texture_desc, sizeof(texture_desc));

    texture_desc.Width                = scd.BufferDesc.Width;
    texture_desc.Height               = scd.BufferDesc.Height;
    texture_desc.MipLevels            = 1;
    texture_desc.ArraySize            = 1;
    texture_desc.Format               = DXGI_FORMAT_R24G8_TYPELESS;
    texture_desc.SampleDesc.Count     = 1;
    texture_desc.Usage                = D3D11_USAGE_DEFAULT;
    texture_desc.CPUAccessFlags       = 0;
    texture_desc.MiscFlags            = 0;
    texture_desc.BindFlags            = D3D11_BIND_DEPTH_STENCIL;
    hr = device_->CreateTexture2D(&texture_desc, NULL, texture.GetAddressOf());

    D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_desc;
    ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));
    depth_stencil_desc.Format         = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depth_stencil_desc.ViewDimension  = D3D11_DSV_DIMENSION_TEXTURE2D;
    hr = device_->CreateDepthStencilView(texture.Get(), &depth_stencil_desc, &dsv_);
    texture = nullptr;

    return hr;
}

void Device::CreateViewport()
{
    viewport_.Width     = g_rectClient.right;
    viewport_.Height    = g_rectClient.bottom;
    viewport_.TopLeftX  = 0;
    viewport_.TopLeftY  = 0;
    viewport_.MinDepth  = 0.0f;
    viewport_.MaxDepth  = 1.0f;
    device_context_->RSSetViewports(1, &viewport_);
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
    device_context_->OMSetRenderTargets(0, nullptr, NULL);
    rtv_ = nullptr;
    dsv_ = nullptr;

    // resize back buffer size
    DXGI_SWAP_CHAIN_DESC new_swapchain_desc;
    swapchain_->GetDesc(&new_swapchain_desc);
    hr = swapchain_->ResizeBuffers(new_swapchain_desc.BufferCount, width, height, new_swapchain_desc.BufferDesc.Format, 0);

    // recreate render target view
    if (FAILED(CreateRenderTargetView())) {
        return S_FALSE;
    }
    if (FAILED(CreateDepthStencilView())) {
        return S_FALSE;
    }

    CreateViewport();
    CreateDXResource();
    return hr;
}
