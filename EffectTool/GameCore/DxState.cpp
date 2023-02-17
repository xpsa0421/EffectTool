#include "DxState.h"

void ID3D11State::Init()
{
    device_         = nullptr;
    device_context_ = nullptr;
}

void ID3D11State::SetDevice(ID3D11Device* device, ID3D11DeviceContext* context)
{
    device_         = device;
    device_context_ = context;
}

bool ID3D11State::CreateStates()
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
    device_->CreateSamplerState(&samplerDesc, &SS_wrap_);
    states_.insert(std::make_pair(L"SS_wrap", (ID3D11DeviceChild*)SS_wrap_.Get()));

    samplerDesc.Filter      = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU    = D3D11_TEXTURE_ADDRESS_MIRROR;
    samplerDesc.AddressV    = D3D11_TEXTURE_ADDRESS_MIRROR;
    samplerDesc.AddressW    = D3D11_TEXTURE_ADDRESS_MIRROR;
    device_->CreateSamplerState(&samplerDesc, &SS_mirror_);
    states_.insert(std::make_pair(L"SS_mirror", (ID3D11DeviceChild*)SS_mirror_.Get()));

    samplerDesc.AddressU        = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.AddressV        = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.AddressW        = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.BorderColor[0]  = 1.0f;  samplerDesc.BorderColor[1] = 1.0f;
    samplerDesc.BorderColor[2]  = 1.0f;  samplerDesc.BorderColor[3] = 1.0f;
    device_->CreateSamplerState(&samplerDesc, &SS_border_);
    states_.insert(std::make_pair(L"SS_border", (ID3D11DeviceChild*)SS_border_.Get()));

    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    device_->CreateSamplerState(&samplerDesc, &SS_smooth_);
    states_.insert(std::make_pair(L"SS_smooth", (ID3D11DeviceChild*)SS_smooth_.Get()));

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
    device_->CreateBlendState(&blendDesc, &BS_alpha_);
    states_.insert(std::make_pair(L"BS_alpha", (ID3D11DeviceChild*)BS_alpha_.Get()));

    blendDesc.RenderTarget[0].BlendEnable = FALSE;
    device_->CreateBlendState(&blendDesc, &BS_no_apha_);
    states_.insert(std::make_pair(L"BS_no_apha", (ID3D11DeviceChild*)BS_no_apha_.Get()));

    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
    device_->CreateBlendState(&blendDesc, &BS_one_zero_);
    states_.insert(std::make_pair(L"BS_one_zero", (ID3D11DeviceChild*)BS_one_zero_.Get()));

    blendDesc.AlphaToCoverageEnable = TRUE;
    blendDesc.IndependentBlendEnable = TRUE;
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC1_COLOR;
    device_->CreateBlendState(&blendDesc, &BS_dual_source_blend_);
    states_.insert(std::make_pair(L"BS_dual_source_blend", (ID3D11DeviceChild*)BS_dual_source_blend_.Get()));

    blendDesc.AlphaToCoverageEnable = FALSE;
    device_->CreateBlendState(&blendDesc, &BS_dual_source_no_blend_);
    states_.insert(std::make_pair(L"BS_dual_source_no_blend", (ID3D11DeviceChild*)BS_dual_source_no_blend_.Get()));

    //-----------------------------------------------------------------------------
    // Create rasterizer states
    //-----------------------------------------------------------------------------
    D3D11_RASTERIZER_DESC rasterDesc;
    ZeroMemory(&rasterDesc, sizeof(rasterDesc));
    rasterDesc.DepthClipEnable = TRUE;
    rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
    rasterDesc.CullMode = D3D11_CULL_NONE;
    device_->CreateRasterizerState(&rasterDesc, &RS_wireframe_);
    states_.insert(std::make_pair(L"RS_wireframe", (ID3D11DeviceChild*)RS_wireframe_.Get()));

    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    device_->CreateRasterizerState(&rasterDesc, &RS_solid_);
    states_.insert(std::make_pair(L"RS_solid", (ID3D11DeviceChild*)RS_solid_.Get()));

    //-----------------------------------------------------------------------------
    // Create depth stencil states
    //-----------------------------------------------------------------------------
    D3D11_DEPTH_STENCIL_DESC depthDesc;
    ZeroMemory(&depthDesc, sizeof(depthDesc));
    depthDesc.DepthEnable       = TRUE;
    depthDesc.DepthWriteMask    = D3D11_DEPTH_WRITE_MASK_ALL;
    depthDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    device_->CreateDepthStencilState(&depthDesc, &DS_depth_enable_);
    states_.insert(std::make_pair(L"DS_depth_enable", (ID3D11DeviceChild*)DS_depth_enable_.Get()));

    depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    device_->CreateDepthStencilState(&depthDesc, &DS_depth_enable_no_write_);
    states_.insert(std::make_pair(L"DS_depth_enable_no_write", (ID3D11DeviceChild*)DS_depth_enable_no_write_.Get()));

    depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthDesc.DepthFunc = D3D11_COMPARISON_GREATER;
    device_->CreateDepthStencilState(&depthDesc, &DS_depth_greater_);
    states_.insert(std::make_pair(L"DS_depth_greater", (ID3D11DeviceChild*)DS_depth_greater_.Get()));

    depthDesc.DepthEnable = FALSE;
    device_->CreateDepthStencilState(&depthDesc, &DS_depth_disable_);
    states_.insert(std::make_pair(L"DS_depth_disable", (ID3D11DeviceChild*)DS_depth_disable_.Get()));

    return true;
}

bool ID3D11State::Release()
{
    for (auto iter = states_.begin(); iter != states_.end(); iter++)
    {
        iter->second = nullptr;
    }
    states_.clear();

    return true;
}

void ID3D11State::ApplySamplerState(W_STR state_name, UINT startSlot, UINT numSamplers)
{
    auto state_iter = states_.find(state_name);

    if (state_iter != states_.end())
    {
        ID3D11SamplerState* state = (ID3D11SamplerState*)(state_iter->second);
        device_context_->PSSetSamplers(startSlot, numSamplers, &state);
    }
    else
    {
#ifdef _DEBUG
        OutputDebugStringA(("Failed to apply sampler state: " + wtm(state_name) + "\n").c_str());
#endif // _DEBUG
    }
}

void ID3D11State::ApplyBlendState(W_STR state_name, const FLOAT blendFactor[], UINT sampleMask)
{
    auto state_iter = states_.find(state_name);

    if (state_iter != states_.end())
    {
        ID3D11BlendState* state = (ID3D11BlendState*)state_iter->second;
        device_context_->OMSetBlendState(state, blendFactor, sampleMask);
    }
    else
    {
#ifdef _DEBUG
        OutputDebugStringA(("Failed to apply blend state: " + wtm(state_name) + "\n").c_str());
#endif // _DEBUG
    }
}

void ID3D11State::ApplyRasterizerState(W_STR state_name)
{
    auto state_iter = states_.find(state_name);

    if (state_iter != states_.end())
    {
        ID3D11RasterizerState* state = (ID3D11RasterizerState*)state_iter->second;
        device_context_->RSSetState(state);
    }
    else
    {
#ifdef _DEBUG
        OutputDebugStringA(("Failed to apply rasterizer state: " + wtm(state_name) + "\n").c_str());
#endif // _DEBUG
    }
}

void ID3D11State::ApplyDepthStencilState(W_STR state_name, UINT stencilRef)
{
    auto state_iter = states_.find(state_name);

    if (state_iter != states_.end())
    {
        ID3D11DepthStencilState* state = (ID3D11DepthStencilState*)state_iter->second;
        device_context_->OMSetDepthStencilState(state, stencilRef);
    }
    else
    {
#ifdef _DEBUG
        OutputDebugStringA(("Failed to apply depth stencil state: " + wtm(state_name) + "\n").c_str());
#endif // _DEBUG
    }
}
