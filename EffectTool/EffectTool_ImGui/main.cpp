#include "Std.h"
#include "EffectTool.h"

void ImGuiRender(EffectTool* effect_tool);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    EffectTool effect_tool;
    effect_tool.SetWindow(hInstance, L"Effect tool", 1280, 800);

	// Initialisation
	effect_tool.CoreInit();

	// Application main loop
	while (effect_tool.game_active_)
	{
		if (effect_tool.window_->Run() == true)
		{
			effect_tool.CoreFrame();
			ImGuiRender(&effect_tool);
			effect_tool.CoreRender();
		}
		else {
			effect_tool.game_active_ = false;
		}
	}

	effect_tool.CoreRelease();
}

void ImGuiRender(EffectTool* effect_tool)
{
	bool show_demo_window = true;
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);
}