#include "EffectTool.h"

bool EffectTool::Init()
{
    // initialise camera
    cam_ = new Camera;
    cam_->Init();
    cam_->SetView(XMFLOAT3(0, 0, -20), XMFLOAT3(0, 0, 0));
    cam_->SetLens(1.0f, 10000.0f, XM_PI * 0.25f,
        (float)g_rectClient.right / (float)g_rectClient.bottom);

    //********************************************************************
    // initialise object
    tex_anim_ps_ = new ParticleSystem;
    tex_anim_ps_->Init();
    
    //template code for adding textures as multiple fies
    std::vector<W_STR> tex_names = {
        L"../../data/image/shine0.bmp",
        L"../../data/image/shine1.bmp",
        L"../../data/image/shine2.bmp",
        L"../../data/image/shine3.bmp",
        L"../../data/image/shine4.bmp",
        L"../../data/image/shine5.bmp",
        L"../../data/image/shine6.bmp",
        L"../../data/image/shine7.bmp",
        L"../../data/image/shine8.bmp",
        L"../../data/image/shine9.bmp"
    };
    tex_anim_ps_->emitter_pos_ = { -6, 0, 0 };
    tex_anim_ps_->SetLifetimeOffset(3,5);
    tex_anim_ps_->SetEmitterProperties(1.0f, 10);
    tex_anim_ps_->EmitParticles();
    tex_anim_ps_->Create(device_.Get(), device_context_.Get());
    tex_anim_ps_->SetMultiTexAnimation(tex_names);
    //********************************************************************

     // initialise object
   /* uv_anim_ps_ = new ParticleSystem;
    uv_anim_ps_->Init();
    uv_anim_ps_->emitter_pos_ = { 5, 0, 0 };
    uv_anim_ps_->SetEmitterProperties(-1, 1);
    uv_anim_ps_->EmitParticles();
    uv_anim_ps_->Create(device_.Get(), device_context_.Get());
    uv_anim_ps_->SetUVAnimation(L"../../data/image/fire.dds", 4, 4);*/





    
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
   
    tex_anim_ps_->Frame();
   // uv_anim_ps_->Frame();

	return true;
}

bool EffectTool::Render()
{
    device_context_->UpdateSubresource(gs_cbuffer_per_frame_.Get(), 0, 0, &gs_cdata_per_frame_, 0, 0);
    device_context_->GSSetConstantBuffers(0, 1, gs_cbuffer_per_frame_.GetAddressOf());

    tex_anim_ps_->Render();
   // uv_anim_ps_->Render();
	return true;
}

bool EffectTool::Release()
{
    tex_anim_ps_->Release();
    delete tex_anim_ps_;
    
   // uv_anim_ps_->Release();
   // delete uv_anim_ps_;

    gs_cbuffer_per_frame_ = nullptr;

    return true;
}


// EffectTool function
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    EffectTool effect_tool;
    effect_tool.SetWindow(hInstance, L"effect_tool", 800, 600);
    effect_tool.Run();

    return 1;
}