#include "Main.h"

bool Main::Init()
{
    // initialise camera
    cam_ = new Camera;
    cam_->Init();
    cam_->SetView(XMFLOAT3(0, 3.8, -13), XMFLOAT3(0, 0, 0));
    cam_->SetLens(1.0f, 10000.0f, XM_PI * 0.25f,
        (float)g_rectClient.right / (float)g_rectClient.bottom);

    // initialise object
    box_ = new Object;
    box_->Init();
    box_->SetVertexShader(L"VertexShader.hlsl", L"main");
    box_->SetPixelShader(L"PixelShader.hlsl", L"main");
    box_->Create(device_.Get(), device_context_.Get());
    
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

    box_->Frame();
    box_->SetTransformationMatrix(nullptr, &cam_->view_, &cam_->proj_);

	return true;
}

bool Main::Render()
{
    gs_cdata_per_frame_.eye_pos = cam_->pos_;
    device_context_->UpdateSubresource(gs_cbuffer_per_frame_.Get(), 0, 0, &gs_cdata_per_frame_, 0, 0);
    device_context_->GSSetConstantBuffers(0, 1, gs_cbuffer_per_frame_.GetAddressOf());

    box_->Render();
	return true;
}

bool Main::Release()
{
    box_->Release();
    delete box_;

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