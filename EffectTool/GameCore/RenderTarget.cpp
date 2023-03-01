#include "RenderTarget.h"

bool RenderTarget::Create(ID3D11Device* device, FLOAT width, FLOAT height)
{
    device->GetImmediateContext(device_context_.GetAddressOf());

    // Set clear color
    clear_color_[0] = 1.0f;
    clear_color_[1] = 1.0f;
    clear_color_[2] = 1.0f;
    clear_color_[3] = 0.9f;

    HRESULT hr;
    viewport_.Width = width;
    viewport_.Height = height;
    viewport_.TopLeftX = 0;
    viewport_.TopLeftY = 0;
    viewport_.MinDepth = 0.0f;
    viewport_.MaxDepth = 1.0f;

    D3D11_TEXTURE2D_DESC texture_desc;
    texture_desc.Width = (UINT)width;
    texture_desc.Height = (UINT)height;
    texture_desc.MipLevels = 1;
    texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texture_desc.SampleDesc.Count = 1;
    texture_desc.SampleDesc.Quality = 0;
    texture_desc.Usage = D3D11_USAGE_DEFAULT;
    texture_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    texture_desc.CPUAccessFlags = 0;
    texture_desc.MiscFlags = 0;
    texture_desc.ArraySize = 1;

    if (FAILED(hr = device->CreateTexture2D(&texture_desc, NULL, texture2D_.GetAddressOf()))) {
        return false;
    }
    if (FAILED(hr = device->CreateShaderResourceView(texture2D_.Get(), NULL, srv_.GetAddressOf()))) {
        return false;
    }
    if (FAILED(hr = device->CreateRenderTargetView(texture2D_.Get(), NULL, rtv_.GetAddressOf()))) {
        return false;
    }

    ComPtr<ID3D11Texture2D> texture2D;
    D3D11_TEXTURE2D_DESC depth_stencil_desc;
    depth_stencil_desc.Width = width;
    depth_stencil_desc.Height = height;
    depth_stencil_desc.MipLevels = 1;
    depth_stencil_desc.ArraySize = 1;
    depth_stencil_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    depth_stencil_desc.SampleDesc.Count = 1;
    depth_stencil_desc.SampleDesc.Quality = 0;
    depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
    depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    depth_stencil_desc.CPUAccessFlags = 0;
    depth_stencil_desc.MiscFlags = 0;
    if (FAILED(hr = device->CreateTexture2D(&depth_stencil_desc, NULL, texture2D.GetAddressOf())))
    {
        return false;
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    if (FAILED(hr = device->CreateDepthStencilView(texture2D.Get(), &dsvDesc, dsv_.GetAddressOf())))
    {
        return false;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    if (FAILED(hr = device->CreateShaderResourceView(texture2D.Get(), &srvDesc, depth_srv_.GetAddressOf())))
    {
        return false;
    }
    
    return true;
}

bool RenderTarget::Begin()
{
    ID3D11RenderTargetView* nullRtv = NULL;
    UINT num_vp = 1;

    device_context_->RSGetViewports(&num_vp, viewport_old_);
    device_context_->OMGetRenderTargets(1, &rtv_old_, &dsv_old_);
    device_context_->OMSetRenderTargets(1, &nullRtv, NULL);
    device_context_->OMSetRenderTargets(1, rtv_.GetAddressOf(), dsv_.Get());
    device_context_->ClearRenderTargetView(rtv_.Get(), clear_color_);
    device_context_->ClearDepthStencilView(dsv_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);
    device_context_->RSSetViewports(1, &viewport_);
    return true;
}

void RenderTarget::End()
{
    device_context_->OMSetRenderTargets(1, &rtv_old_, dsv_old_);
    device_context_->RSSetViewports(1, viewport_old_);
    rtv_old_->Release();
    dsv_old_->Release();
}

bool RenderTarget::Release()
{
    srv_            = nullptr;
    rtv_            = nullptr;
    dsv_            = nullptr;
    depth_srv_      = nullptr;
    texture2D_      = nullptr;
    rtv_old_        = nullptr;
    dsv_old_        = nullptr;
    device_context_ = nullptr;

    return true;
}

void RenderTarget::Resize(FLOAT width, FLOAT height)
{
    viewport_.Width = width;
    viewport_.Height = height;
}

void RenderTarget::SetClearColor(const FLOAT color[4])
{
    clear_color_[0] = color[0];
    clear_color_[1] = color[1];
    clear_color_[2] = color[2];
    clear_color_[3] = color[3];
}