#include "Main.h"

bool Main::Init()
{
    // initialise camera
    cam_ = new Camera;
    cam_->Init();
    cam_->SetView(XMFLOAT3(3, 0, -10), XMFLOAT3(3, 0, 0));
    cam_->SetLens(1.0f, 10000.0f, XM_PI * 0.25f,
        (float)g_rectClient.right / (float)g_rectClient.bottom);

    // initialise object
    particle_system_ = new ParticleSystem;
    particle_system_->Init();
    particle_system_->SetVertexShader(L"VertexShader.hlsl", L"main");
    particle_system_->SetPixelShader(L"PixelShader.hlsl", L"main");
    particle_system_->SetGeometryShader(L"GeometryShader.hlsl", L"main");
    particle_system_->Create(device_.Get(), device_context_.Get());
    
    // generate per frame geometry shader constant buffer
    D3D11_BUFFER_DESC constant_desc;
    ZeroMemory(&constant_desc, sizeof(constant_desc));
    constant_desc.ByteWidth = sizeof(CdPerParticleSystem) * 1;
    constant_desc.Usage = D3D11_USAGE_DEFAULT;
    constant_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    HRESULT result = device_->CreateBuffer(&constant_desc, nullptr, gs_cbuffer_per_frame_.GetAddressOf());
    #ifdef _DEBUG
    if (FAILED(result))
    {
        printf("[Main] Failed to build per frame GS constant buffer\n");
    }
    #endif // _DEBUG

    return true;
}

bool Main::Frame()
{
    cam_->Frame();

    // generate billboard matrix
    XMFLOAT4X4 cam_view_in;
    XMStoreFloat4x4(&cam_view_in, XMMatrixInverse(nullptr, XMLoadFloat4x4(&cam_->view_)));
    cam_view_in._41 = cam_view_in._42 = cam_view_in._43 = 0.0f;

    particle_system_->Update(&cam_view_in, &cam_->view_, &cam_->proj_);
    particle_system_->Frame();

	return true;
}

bool Main::Render()
{
    gs_cdata_per_frame_.eye_pos = cam_->pos_;
    device_context_->UpdateSubresource(gs_cbuffer_per_frame_.Get(), 0, 0, &gs_cdata_per_frame_, 0, 0);
    device_context_->GSSetConstantBuffers(0, 1, gs_cbuffer_per_frame_.GetAddressOf());

    particle_system_->Render();
	return true;
}

bool Main::Release()
{
    particle_system_->Release();
    delete particle_system_;

    return true;
}

// main function
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    Main coreMain;
    coreMain.SetWindow(hInstance, L"ParticleTester", 800, 600);
    coreMain.Run();

    return 1;
}