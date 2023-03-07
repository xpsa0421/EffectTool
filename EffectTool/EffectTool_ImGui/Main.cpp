#define _CRT_SECURE_NO_WARNINGS
#include "EffectTool.h"
#include "ImGuiRenderer.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	// Create an instance of the effect tool
    EffectTool effect_tool;
    effect_tool.SetWindow(hInstance, L"Effect tool", 1280, 800);
	
	// Create an instance of ImGui renderer
	ImGuiRenderer imgui_renderer;

	// Initialisation
	effect_tool.CoreInit();
	imgui_renderer.Init(&effect_tool);
	
	// Application main loop
	while (effect_tool.game_active_)
	{
		if (effect_tool.window_->Run() == true)
		{
			// Update and render core
			effect_tool.CoreFrame();
			effect_tool.CoreRender();

			// Update and render ImGui
			ImGuiLayer::Begin();
			imgui_renderer.Render();
			ImGuiLayer::End();

			effect_tool.swapchain_->Present(0, 0);
		}
		else {
			effect_tool.game_active_ = false;
		}
	}
	// Release
	effect_tool.CoreRelease();
}