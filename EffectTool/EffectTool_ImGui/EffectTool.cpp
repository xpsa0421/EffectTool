#include "EffectTool.h"

bool EffectTool::Init()
{
    return true;
}

bool EffectTool::Frame()
{
    // update all active particle systems
    for (int ps_idx = 0; ps_idx < particle_systems_.size(); ps_idx++)
    {
        if (ps_window_is_active_[ps_idx])
            particle_systems_[ps_idx]->Frame();
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
}

bool EffectTool::Render()
{
    SetRenderStates();
    
    // Render particle systems on active windows
    for (int ps_idx = 0; ps_idx < particle_systems_.size(); ps_idx++)
    {
        if (ps_window_is_active_[ps_idx])
        {
            render_targets_[ps_idx]->Begin();
            particle_systems_[ps_idx]->Render();
            render_targets_[ps_idx]->End();
        }
    }

	return true;
}

bool EffectTool::Release()
{
    for (ParticleSystem* particle_system : particle_systems_)
    {
        particle_system->Release();
        delete particle_system;
    }
    for (RenderTarget* render_target : render_targets_)
    {
        render_target->Release();
        delete render_target;
    }

    return true;
}

ID3D11ShaderResourceView* EffectTool::GetRenderedTexture(int ps_idx)
{
    return render_targets_[ps_idx]->GetSRV();
}

int EffectTool::GetNumParticleSystems()
{
    return particle_systems_.size();
}

int EffectTool::GetNumEmittersInPS(int ps_idx)
{
    return particle_systems_[ps_idx]->GetNumEmitters();
}

W_STR EffectTool::GetPSName(int ps_idx)
{
    return particle_systems_[ps_idx]->name_;
}

void EffectTool::SetPSName(int ps_idx, W_STR name)
{
    particle_systems_[ps_idx]->name_ = name;
}

void EffectTool::ResizeViewport(int ps_idx, float width, float height)
{
    render_targets_[ps_idx]->Resize(width, height); // TODO:: fix flickering
    particle_systems_[ps_idx]->SetCameraLens(1.0f, 10000.0f, XM_PI * 0.25f, width / height);
}

void EffectTool::SetPSWindowState(int ps_idx, bool state)
{
    ps_window_is_active_[ps_idx] = state;
}

/**
 * Creates and adds a default particle system, render target, and camera
 */
void EffectTool::CreateParticleSystem(W_STR name)
{
    // Initialise particle system
    particle_systems_.push_back(new ParticleSystem());
    particle_systems_.back()->SetDeviceContext(device_.Get(), device_context_.Get());
    particle_systems_.back()->Init();
    particle_systems_.back()->name_ = name;

    // Initialise rendertarget
    render_targets_.push_back(new RenderTarget());
    render_targets_.back()->Create(device_.Get(), 500, 500);

    // Initialise camera
    Camera* cam = new Camera();
    cam->Init();
    cam->SetView(XMFLOAT3(0, 0, -180), XMFLOAT3(0, 50, 0));
    cam->SetLens(1.0f, 10000.0f, XM_PI * 0.25f, 500 / 500);

    particle_systems_.back()->SetCamera(cam);
    ps_window_is_active_.push_back(true);
}

void EffectTool::AddEmitterToPS(int ps_idx)
{
    particle_systems_[ps_idx]->AddDefaultEmitter();
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
