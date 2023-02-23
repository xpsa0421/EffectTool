#include "pch.h"
#include "EffectTool.h"

bool EffectTool::Init()
{
    ReadEmitterFromFile(L"../../data/emitter/test.json");

    // initialise camera
    cam_ = new Camera;
    cam_->Init();
    cam_->SetView(XMFLOAT3(0, 0, -200), XMFLOAT3(0, 50, 0));
    cam_->SetLens(1.0f, 10000.0f, XM_PI * 0.25f,
        (float)g_rectClient.right / (float)g_rectClient.bottom);

    CreateDefaultEmitter();
    
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
   
    for (auto emitter : emitters)
    {
        emitter.second->Frame();
    }

	return true;
}

bool EffectTool::Render()
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

    // set alpha testing
    if (!dualsource_blended_)
    {
        for (auto emitter : emitters)
            emitter.second->SetPixelShader(L"../../data/shader/PixelShader.hlsl", L"NoDualAlphaBlend");
    }
    else if (alpha_tested_)
    {
        for (auto emitter : emitters)
            emitter.second->SetPixelShader(L"../../data/shader/PixelShader.hlsl", L"AlphaTested");
    }
    else
    {
        for (auto emitter : emitters)
            emitter.second->SetPixelShader(L"../../data/shader/PixelShader.hlsl", L"AlphaNotTested");
    }


    device_context_->UpdateSubresource(gs_cbuffer_per_frame_.Get(), 0, 0, &gs_cdata_per_frame_, 0, 0);
    device_context_->GSSetConstantBuffers(0, 1, gs_cbuffer_per_frame_.GetAddressOf());
    
    W_STR text = L"";
    
    for (auto emitter : emitters)
    {
        emitter.second->Render();
        text += L" " + std::to_wstring(emitter.second->num_alive_particles) + L"/" + 
            std::to_wstring(emitter.second->num_requested_particles) + L"\n";
    }

    writer_.Draw(10, 5, text);

	return true;
}

bool EffectTool::Release()
{
    for (auto emitter : emitters)
    {
        emitter.second->Release();
        delete emitter.second;
    }
    
   // uv_anim_ps_->Release();
   // delete uv_anim_ps_;

    gs_cbuffer_per_frame_ = nullptr;

    return true;
}

HRESULT EffectTool::CreateDXResource()
{
    GameCore::CreateDXResource();
    if (cam_)
    {
        cam_->SetLens(1.0f, 10000.0f, XM_PI * 0.25f,
            (float)g_rectClient.right / (float)g_rectClient.bottom);
    }
    return S_OK;
}

void    EffectTool::GenEmitterFromMultipleTex(float spawn_rate, XMFLOAT3 emitter_pos, XMFLOAT3 pos_offset_min, XMFLOAT3 pos_offset_max,
    XMFLOAT2 size_min, XMFLOAT2 size_max, XMFLOAT2 lifetime_minmax,
    XMFLOAT3 velocity_min, XMFLOAT3 velocity_max, BOOL use_random_color, W_STR emitter_name, std::vector<W_STR> tex_names)
{
    ParticleSystem* emitter = new ParticleSystem;
    emitter->Init();
    //emitter->SetSpawnRate(emit_cycle, num_particles);
    emitter->SetEmitterPos(emitter_pos);
    emitter->SetPosOffset(pos_offset_min, pos_offset_max);
    emitter->SetSizeOffset(size_min, size_max);
    emitter->SetLifetimeOffset(lifetime_minmax.x, lifetime_minmax.y);
    emitter->SetVelocity(velocity_min, velocity_max);
    emitter->SetAdditiveColor(use_random_color);
    emitter->EmitParticle();
    emitter->Create(device_.Get(), device_context_.Get());
   // emitter->SetMultiTexAnimation(tex_names);
    emitter->SetUVAnimation(tex_names[0], 4, 4);

    emitters.insert(std::make_pair(emitter_name, emitter));
}

bool EffectTool::NameExists(W_STR name)
{
    return (emitters.find(name) != emitters.end());
}

void EffectTool::CreateDefaultEmitter()
{
    ParticleSystem* emitter = new ParticleSystem;
    emitter->Init();
    emitter->SetSpawnRate(20);
    emitter->SetEmitterPos({ 0,0,0 });
    emitter->SetSizeOffset({ 25,25 }, { 25,25 });
    emitter->SetLifetimeOffset(1,1);
    emitter->SetVelocity({ -30,50,-10 },{30,100,10});
    emitter->Create(device_.Get(), device_context_.Get());
    emitter->SetTexture(L"../../data/image/OrientParticle.PNG");
    emitters.insert(std::make_pair(L"/default/", emitter));
}

void EffectTool::SetEmitterTexture(W_STR emitter_name, W_STR tex_path,
    int num_rows, int num_cols)
{
    emitters.find(emitter_name)->second->SetUVAnimation(tex_path, num_rows, num_cols);
}

void EffectTool::SetEmitterTexture(W_STR emitter_name, std::vector<W_STR>& tex_paths)
{

}

void EffectTool::SetEmitterTexture(W_STR emitter_name, W_STR tex_path)
{

}

void EffectTool::SetSpawnRate(W_STR emitter_name, float spawn_rate)
{
    auto emitter = emitters.find(emitter_name);
    if (emitter != emitters.end())
    {
        emitter->second->SetSpawnRate(spawn_rate);
    }
}

void EffectTool::UpdateSizeOffset(W_STR emitter_name, XMFLOAT2 size_min, XMFLOAT2 size_max)
{
    auto emitter = emitters.find(emitter_name);
    if (emitter != emitters.end())
    {
        emitter->second->SetSizeOffset(size_min, size_max);
    }
}

bool EffectTool::ReadEmitterFromFile(W_STR filepath)
{
    rapidjson::Document emitter_data;
    
    // load file and check validity
    if (!JsonHelper::LoadJSON(filepath, emitter_data)) return false;

    // declare property variables
    W_STR               emitter_name;
    std::vector<W_STR>  tex_paths;
    XMFLOAT2            tex_rows_cols;
    float               spawn_rate;
    XMFLOAT3            emitter_pos;
    XMFLOAT3            pos_offset_min;
    XMFLOAT3            pos_offset_max;
    XMFLOAT3            initial_size_min;
    XMFLOAT3            initial_size_max;
    XMFLOAT2            lifetime;
    XMFLOAT3            velocity_min;
    XMFLOAT3            velocity_max;
    XMFLOAT4            color;


    // iterate all emitters
    for (auto& emitter : emitter_data.GetObject())
    {
        rapidjson::Value& properties = emitter.value;

        // check for incorrect emitter data
        if (!properties.IsObject() || properties.MemberCount() != EMITTER_PROPERTIES_COUNT)
        {
            return false;
        }

        // retrieve emitter name
        emitter_name = mtw(emitter.name.GetString());

        // retrieve emitter properties
        // retrieve texture paths
        auto texpath_itr = properties.FindMember("tex_paths");
        if (texpath_itr == properties.MemberEnd())
        {
            return false;
        }
        rapidjson::Value& texpath_list = texpath_itr->value;
        if (!texpath_list.IsArray() || texpath_list.Size() == 0)
        {
            return false;
        }
        for (rapidjson::SizeType i = 0; i < texpath_list.Size(); i++)
        {
            if (!texpath_list[i].IsString())
            {
                return false;
            }
            tex_paths.push_back(mtw(texpath_list[i].GetString()));
        }

        // retrieve number of rows and columns in texture
        if (!JsonHelper::GetFloat2(properties, "tex_rows_cols", tex_rows_cols))
        {
            return false;
        }

        // retrieve spawn rate
        if (!JsonHelper::GetFloat(properties, "spawn_rate", spawn_rate))
        {
            return false;
        }
        
        // retrieve particle lifetime min and max
        if (!JsonHelper::GetFloat2(properties, "lifetime", lifetime))
        {
            return false;
        }
        
        // retrieve emitter position
        if (!JsonHelper::GetFloat3(properties, "emitter_pos", emitter_pos))
        {
            return false;
        }
        
        // retrieve minimum particle position offset 
        if (!JsonHelper::GetFloat3(properties, "pos_offset_min", pos_offset_min))
        {
            return false;
        }

        // retrieve maximum particle position offset 
        if (!JsonHelper::GetFloat3(properties, "pos_offset_max", pos_offset_max))
        {
            return false;
        }
        
        // retrieve minimum particle initial size
        if (!JsonHelper::GetFloat3(properties, "initial_size_min", initial_size_min))
        {
            return false;
        }

        // retrieve maximum particle initial size
        if (!JsonHelper::GetFloat3(properties, "initial_size_max", initial_size_max))
        {
            return false;
        }

        // retrieve minimum particle initial velocity
        if (!JsonHelper::GetFloat3(properties, "initial_velocity_min", velocity_min))
        {
            return false;
        }

        // retrieve maximum particle initial velocity
        if (!JsonHelper::GetFloat3(properties, "initial_velocity_max", velocity_max))
        {
            return false;
        }

        // retrieve particle color
        if (!JsonHelper::GetFloat4(properties, "color", color))
        {
            return false;
        }
    }

    // do something with the values
    return true;
}