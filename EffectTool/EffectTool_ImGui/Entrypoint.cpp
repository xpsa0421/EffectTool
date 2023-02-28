#include "Std.h"
#include "EffectTool.h"
#include "imgui.h"

void ImGuiRender(EffectTool* effect_tool);

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

	// Initialisation
	effect_tool.CoreInit();

	// Application main loop
	while (effect_tool.game_active_)
	{
		if (effect_tool.window_->Run() == true)
		{
			// Render mainframe
			effect_tool.CoreFrame();
			effect_tool.CoreRender();

			// Render ImGui
			ImGuiLayer::Begin();
			ImGuiRender(&effect_tool);
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

void ImGuiRender(EffectTool* effect_tool)
{
	bool show_demo_window = true;
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);
}