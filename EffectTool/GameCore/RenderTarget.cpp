#include "RenderTarget.h"

bool RenderTarget::Create(ID3D11Device* device, FLOAT width, FLOAT height)
{
    HRESULT hr;
    _viewport.Width = width;
    _viewport.Height = height;
    _viewport.TopLeftX = 0;
    _viewport.TopLeftY = 0;
    _viewport.MinDepth = 0.0f;
    _viewport.MaxDepth = 1.0f;


    _textureDesc.Width = (UINT)width;
    _textureDesc.Height = (UINT)height;
    _textureDesc.MipLevels = 1;
    _textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    _textureDesc.SampleDesc.Count = 1;
    _textureDesc.SampleDesc.Quality = 0;
    _textureDesc.Usage = D3D11_USAGE_DEFAULT;
    _textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    _textureDesc.CPUAccessFlags = 0;
    _textureDesc.MiscFlags = 0;
    _textureDesc.ArraySize = 1;

    if (FAILED(hr = device->CreateTexture2D(&_textureDesc, NULL, &_texture2D))) {
        return false;
    }
    if (FAILED(hr = device->CreateShaderResourceView(_texture2D, NULL, &_shaderResourceView))) {
        return false;
    }
    if (FAILED(hr = device->CreateRenderTargetView(_texture2D, NULL, &_renderTargetView))) {
        return false;
    }

    ID3D11Texture2D* texture2D = nullptr;
    D3D11_TEXTURE2D_DESC descDepth;
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_R24G8_TYPELESS;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    if (FAILED(hr = device->CreateTexture2D(&descDepth, NULL, &texture2D)))
    {
        return false;
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    if (FAILED(hr = device->CreateDepthStencilView(texture2D, &dsvDesc, &_dsv)))
    {
        return false;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    if (FAILED(hr = device->CreateShaderResourceView(texture2D, &srvDesc, &_depthSRV)))
    {
        return false;
    }

    texture2D->Release();
    texture2D = nullptr;

    return true;
}

bool RenderTarget::Begin(ID3D11DeviceContext* context)
{
    ID3D11RenderTargetView* nullRTV = NULL;
    context->OMSetRenderTargets(1, &nullRTV, NULL);
    context->OMSetRenderTargets(1, &_renderTargetView, _dsv);
    const FLOAT color[] = { 0.5f,0.5f,0.5f,1 };
    context->ClearRenderTargetView(_renderTargetView, color);
    context->ClearDepthStencilView(_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);
    context->RSSetViewports(1, &_viewport);
    return true;
}

void RenderTarget::End(ID3D11DeviceContext* context)
{
    context->OMSetRenderTargets(1, &_oldRenderTarget, _oldDepthStencil);
    context->RSSetViewports(1, _oldViewPort);
}

bool RenderTarget::Release()
{
    if (_renderTargetView) 
    {
        _renderTargetView->Release();
    }
    if (_dsv) 
    {
        _dsv->Release();
    }
    if (_shaderResourceView) 
    {
        _shaderResourceView->Release();
    }
    if (_depthSRV)
    {
        _depthSRV->Release();
    }
    if (_texture2D) 
    {
        _texture2D->Release();
    }

    _renderTargetView   = nullptr;
    _dsv                = nullptr;
    _shaderResourceView = nullptr;
    _depthSRV           = nullptr;
    _texture2D          = nullptr;
    return true;
}
