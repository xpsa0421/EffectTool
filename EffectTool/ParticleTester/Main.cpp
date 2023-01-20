#include "Main.h"

bool Main::Init()
{
    return true;
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