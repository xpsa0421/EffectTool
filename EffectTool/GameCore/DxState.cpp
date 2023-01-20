#include "DxState.h"
ID3D11SamplerState*         DxState::g_SSWrap           = nullptr;
ID3D11SamplerState*         DxState::g_SSMirror         = nullptr;
ID3D11SamplerState*         DxState::g_SSBorder         = nullptr;
ID3D11SamplerState*         DxState::g_SSSmooth         = nullptr;
ID3D11BlendState*           DxState::g_BSAlpha          = nullptr;
ID3D11BlendState*           DxState::g_BSNoAlpha        = nullptr;
ID3D11BlendState*           DxState::g_BSOneZero        = nullptr;
ID3D11RasterizerState*      DxState::g_RSWireFrame      = nullptr;
ID3D11RasterizerState*      DxState::g_RSSolid          = nullptr;
ID3D11DepthStencilState*    DxState::g_DSDepthDisable   = nullptr;
ID3D11DepthStencilState*    DxState::g_DSDepthEnable    = nullptr;
ID3D11DepthStencilState*    DxState::g_DSDepthGreater   = nullptr;

bool DxState::CreateStates(ID3D11Device* device)
{
    //-----------------------------------------------------------------------------
    // Create sampler states
    //-----------------------------------------------------------------------------
    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(samplerDesc));
    samplerDesc.Filter      = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.AddressU    = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV    = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW    = D3D11_TEXTURE_ADDRESS_WRAP;
    device->CreateSamplerState(&samplerDesc, &g_SSWrap);

    samplerDesc.Filter      = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU    = D3D11_TEXTURE_ADDRESS_MIRROR;
    samplerDesc.AddressV    = D3D11_TEXTURE_ADDRESS_MIRROR;
    samplerDesc.AddressW    = D3D11_TEXTURE_ADDRESS_MIRROR;
    device->CreateSamplerState(&samplerDesc, &g_SSMirror);

    samplerDesc.AddressU        = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.AddressV        = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.AddressW        = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.BorderColor[0]  = 1.0f;  samplerDesc.BorderColor[1] = 1.0f;
    samplerDesc.BorderColor[2]  = 1.0f;  samplerDesc.BorderColor[3] = 1.0f;
    device->CreateSamplerState(&samplerDesc, &g_SSBorder);

    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    device->CreateSamplerState(&samplerDesc, &g_SSSmooth);


    //-----------------------------------------------------------------------------
    // Create blend states
    //-----------------------------------------------------------------------------
    D3D11_BLEND_DESC blendDesc;
    ZeroMemory(&blendDesc, sizeof(blendDesc));
    blendDesc.RenderTarget[0].SrcBlend              = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    blendDesc.RenderTarget[0].BlendEnable           = TRUE;
    device->CreateBlendState(&blendDesc, &g_BSAlpha);

    blendDesc.RenderTarget[0].BlendEnable = FALSE;
    device->CreateBlendState(&blendDesc, &g_BSNoAlpha);

    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
    device->CreateBlendState(&blendDesc, &g_BSOneZero);


    //-----------------------------------------------------------------------------
    // Create rasterizer states
    //-----------------------------------------------------------------------------
    D3D11_RASTERIZER_DESC rasterDesc;
    ZeroMemory(&rasterDesc, sizeof(rasterDesc));
    rasterDesc.DepthClipEnable = TRUE;
    rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
    rasterDesc.CullMode = D3D11_CULL_NONE;
    device->CreateRasterizerState(&rasterDesc, &g_RSWireFrame);

    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    device->CreateRasterizerState(&rasterDesc, &g_RSSolid);

    //-----------------------------------------------------------------------------
    // Create depth stencil states
    //-----------------------------------------------------------------------------
    D3D11_DEPTH_STENCIL_DESC depthDesc;
    ZeroMemory(&depthDesc, sizeof(depthDesc));
    depthDesc.DepthEnable       = TRUE;
    depthDesc.DepthWriteMask    = D3D11_DEPTH_WRITE_MASK_ALL;
    depthDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    device->CreateDepthStencilState(&depthDesc, &g_DSDepthEnable);

    depthDesc.DepthFunc = D3D11_COMPARISON_GREATER;
    device->CreateDepthStencilState(&depthDesc, &g_DSDepthGreater);

    depthDesc.DepthEnable = FALSE;
    device->CreateDepthStencilState(&depthDesc, &g_DSDepthDisable);
    return true;
}

bool DxState::Release()
{
    if (g_SSSmooth)             g_SSSmooth->Release();
    if (g_SSWrap)               g_SSWrap->Release();
    if (g_SSMirror)             g_SSMirror->Release();
    if (g_SSBorder)             g_SSBorder->Release();
    if (g_BSAlpha)              g_BSAlpha->Release();
    if (g_BSNoAlpha)            g_BSNoAlpha->Release();
    if (g_BSOneZero)            g_BSOneZero->Release();
    if (g_RSWireFrame)          g_RSWireFrame->Release();
    if (g_RSSolid)              g_RSSolid->Release();
    if (g_DSDepthDisable)       g_DSDepthDisable->Release();
    if (g_DSDepthEnable)        g_DSDepthEnable->Release();
    if (g_DSDepthGreater)       g_DSDepthGreater->Release();

    return true;
}

void DxState::ApplySamplerState(ID3D11DeviceContext* context,
    ID3D11SamplerState* samplerState, UINT startSlot, UINT numSamplers)
{
    context->PSSetSamplers(startSlot, numSamplers, &samplerState);
}

void DxState::ApplyBlendState(ID3D11DeviceContext* context,
    ID3D11BlendState* blendState, const FLOAT blendFactor[], UINT sampleMask)
{
    context->OMSetBlendState(blendState, blendFactor, sampleMask);
}

void DxState::ApplyRasterizerState(ID3D11DeviceContext* context,
    ID3D11RasterizerState* rasterizerState)
{
    context->RSSetState(rasterizerState);
}

void DxState::ApplyDepthStencilState(ID3D11DeviceContext* context,
    ID3D11DepthStencilState* depthStencilState, UINT stencilRef)
{
    context->OMSetDepthStencilState(depthStencilState, stencilRef);
}
