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
    box_->Create(device_.Get(), device_context_.Get(), L"testEffect.fx");

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