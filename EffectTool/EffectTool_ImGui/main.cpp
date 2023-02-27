//#include "Std.h"
#include "EffectTool.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    EffectTool effect_tool;
    effect_tool.SetWindow(hInstance, L"Effect tool", 1280, 800);
    effect_tool.RunImGui();

    return 1;
}
//
//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//    _In_opt_ HINSTANCE hPrevInstance,
//    _In_ LPWSTR    lpCmdLine,
//    _In_ int       nCmdShow)
//{
//	EffectTool effect_tool;
//	effect_tool.SetWindow(hInstance, L"Effect tool", 1280, 800);
//
//	// Setup Dear ImGui context
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImGuiIO& io = ImGui::GetIO();
//	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
//
//	// Setup Dear ImGui style
//	ImGui::StyleColorsDark();
//
//	// Initialisation
//	ImGui_ImplWin32_Init(g_hWnd);
//	ImGui_ImplDX11_Init(effect_tool.device_.Get(), effect_tool.device_context_.Get());
//	effect_tool.CoreInit();
//
//	// Application main loop
//	while (effect_tool.game_active_)
//	{
//		if (effect_tool.window_->Run() == true)
//		{
//			// Frame
//			ImGui_ImplDX11_NewFrame();
//			ImGui_ImplWin32_NewFrame();
//			ImGui::NewFrame();
//			effect_tool.CoreFrame();
//
//			// Render
//			ImGui::Render();
//			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
//			effect_tool.CoreRender();
//		}
//		else {
//			effect_tool.game_active_ = false;
//		}
//	}
//
//	// Release
//	ImGui_ImplDX11_Shutdown();
//	ImGui_ImplWin32_Shutdown();
//	ImGui::DestroyContext();
//	effect_tool.CoreRelease();
//
//	return true;
//}