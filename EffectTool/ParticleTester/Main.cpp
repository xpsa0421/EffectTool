#include "Main.h"

bool Main::Init()
{
    // initialise camera
    cam_ = new Camera;
    cam_->SetPosition(0, 3.8, -13);
    cam_->SetLens(1.0f, 10000.0f, XM_PI * 0.25f,
        (float)g_rectClient.right / (float)g_rectClient.bottom);

	BuildEnvironment();
    return true;
}

void Main::BuildEnvironment()
{
	
}

bool Main::Frame()
{
	return true;
}

bool Main::Render()
{
	return true;
}

bool Main::Release()
{
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