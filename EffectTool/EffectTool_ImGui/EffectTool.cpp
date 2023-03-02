#include "EffectTool.h"

bool EffectTool::Init()
{
    // initialise rendertarget
    //render_plane_ = new Object();
    //render_plane_->Init();
    //render_plane_->Create(device_.Get(), device_context_.Get());

    render_target_ = new RenderTarget();
    render_target_->Create(device_.Get(), 500, 500);

    // initialise camera
    cam_ = new Camera;
    cam_->Init();
    cam_->SetView(XMFLOAT3(0, 0, -180), XMFLOAT3(0, 50, 0));
    cam_->SetLens(1.0f, 10000.0f, XM_PI * 0.25f, 500/500);
    render_target_size = { 500, 500 };

    // add default particle system upon launch
    AddDefaultParticleSystem();
    
    // generate per frame geometry shader constant buffer
    D3D11_BUFFER_DESC constant_desc;
    ZeroMemory(&constant_desc, sizeof(constant_desc));
    constant_desc.ByteWidth = sizeof(CdPerFrame) * 1;
    constant_desc.Usage = D3D11_USAGE_DEFAULT;
    constant_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    HRESULT result = device_->CreateBuffer(&constant_desc, nullptr, gs_cbuffer_per_frame_.GetAddressOf());
    #ifdef _DEBUG
    if (FAILED(result))
    {
        printf("[EffectTool] Failed to build per frame GS constant buffer\n");
    }
    #endif // _DEBUG

    return true;
}

bool EffectTool::Frame()
{
    cam_->Frame();

    // generate billboard matrix
    XMFLOAT4X4 cam_view_in;
    XMStoreFloat4x4(&cam_view_in, XMMatrixInverse(nullptr, XMLoadFloat4x4(&cam_->view_)));
    //cam_view_in._12 = cam_view_in._22 = cam_view_in._32 = 0.0f;
    cam_view_in._41 = cam_view_in._42 = cam_view_in._43 = 0.0f;
    
    XMFLOAT4X4 cdata_billboard;
    XMFLOAT4X4 cdata_view;
    XMFLOAT4X4 cdata_proj;

    XMStoreFloat4x4(&cdata_billboard, XMMatrixTranspose(XMLoadFloat4x4(&cam_view_in)));
    XMStoreFloat4x4(&cdata_view, XMMatrixTranspose(XMLoadFloat4x4(&cam_->view_)));
    XMStoreFloat4x4(&cdata_proj, XMMatrixTranspose(XMLoadFloat4x4(&cam_->proj_)));

    gs_cdata_per_frame_.billboard = cdata_billboard;
    gs_cdata_per_frame_.view = cdata_view;
    gs_cdata_per_frame_.proj = cdata_proj;
   
    // update all loaded particle systems
    for (ParticleSystem* particle_system : particle_systems)
    {
        particle_system->Frame(); 
    }

	return true;
}

void EffectTool::SetRenderStates()
{
    // set wireframe properties
    if (wireframe_enabled_)
        DxState.ApplyRasterizerState(L"RS_wireframe");
    else
        DxState.ApplyRasterizerState(L"RS_solid");

    // set depth stencil state
    if (depth_compared_)
    {
        if (depth_write_enabled_)
            DxState.ApplyDepthStencilState(L"DS_depth_enable");
        else
            DxState.ApplyDepthStencilState(L"DS_depth_enable_no_write");
    }
    else DxState.ApplyDepthStencilState(L"DS_depth_disable");

    // set alpha blendstate
    if (!dualsource_blended_)
        DxState.ApplyBlendState(L"BS_alpha");
    else if (alpha_blended_)
        DxState.ApplyBlendState(L"BS_dual_source_blend");
    else
        DxState.ApplyBlendState(L"BS_dual_source_no_blend");

    // TODO: To set properties by emitters and update independently
    // set alpha testing
    for (ParticleSystem* particle_system : particle_systems)
    {
        for (ParticleEmitter* emitter : particle_system->emitters_)
        {
            if (!dualsource_blended_)
                emitter->SetPixelShader(L"../../data/shader/PixelShader.hlsl", L"NoDualAlphaBlend");
            else if (alpha_tested_)
                emitter->SetPixelShader(L"../../data/shader/PixelShader.hlsl", L"AlphaTested");
            else
                emitter->SetPixelShader(L"../../data/shader/PixelShader.hlsl", L"AlphaNotTested");
        }
    }
}

bool EffectTool::Render()
{
    SetRenderStates();
    device_context_->UpdateSubresource(gs_cbuffer_per_frame_.Get(), 0, 0, &gs_cdata_per_frame_, 0, 0);
    device_context_->GSSetConstantBuffers(0, 1, gs_cbuffer_per_frame_.GetAddressOf());
   
    render_target_->Begin();
    for (ParticleSystem* particle_system : particle_systems)
    {
        particle_system->Render();
    }
    render_target_->End();
    
    //render_plane_->SetTextureSRV(render_target_->srv_.Get());
    //render_plane_->Render();

	return true;
}

bool EffectTool::Release()
{
    for (ParticleSystem* particle_system : particle_systems)
    {
        particle_system->Release();
        delete particle_system;
    }
    gs_cbuffer_per_frame_ = nullptr;
    render_target_->Release();
    //render_plane_->Release();

    return true;
}

ID3D11ShaderResourceView* EffectTool::GetRenderedTexture()
{
    return render_target_->GetSRV();
}

HRESULT EffectTool::CreateDXResource()
{
    GameCore::CreateDXResource();
    if (cam_)
    {
        cam_->SetLens(1.0f, 10000.0f, XM_PI * 0.25f,
            render_target_size.x / render_target_size.y);
    }
    return S_OK;
}

void EffectTool::ResizeViewport(float width, float height)
{
    render_target_size = { width, height };
    render_target_->Resize(width, height);
    cam_->SetLens(1.0f, 10000.0f, XM_PI * 0.25f, render_target_size.x / render_target_size.y);
}

/**
 * When the tool is launched, create a default particle system
 */
void EffectTool::AddDefaultParticleSystem()
{
    ParticleSystem* default_particle_system = new ParticleSystem();
    default_particle_system->SetDeviceContext(device_.Get(), device_context_.Get());
    default_particle_system->Init();
    particle_systems.push_back(default_particle_system);
}

void EffectTool::SetEmitterTexture(W_STR emitter_name, W_STR tex_path,
    int num_rows, int num_cols)
{
    //emitters.find(emitter_name)->second->SetUVAnimation(tex_path, num_rows, num_cols);
}

void EffectTool::SetEmitterTexture(W_STR emitter_name, std::vector<W_STR>& tex_paths)
{

}

void EffectTool::SetEmitterTexture(W_STR emitter_name, W_STR tex_path)
{

}

void EffectTool::SetSpawnRate(W_STR emitter_name, float spawn_rate)
{
    /*auto emitter = emitters.find(emitter_name);
    if (emitter != emitters.end())
    {
        emitter->second->SetSpawnRate(spawn_rate);
    }*/
}

void EffectTool::UpdateSizeOffset(W_STR emitter_name, XMFLOAT2 size_min, XMFLOAT2 size_max)
{
    /*auto emitter = emitters.find(emitter_name);
    if (emitter != emitters.end())
    {
        emitter->second->SetSizeOffset(size_min, size_max);
    }*/
}
