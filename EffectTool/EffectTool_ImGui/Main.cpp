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
			effect_tool.CoreFrame();

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
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), dockspace_flags);
	bool show_demo_window = true;
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// Main viewport 
	ImGui::SetNextWindowSize(ImVec2(450, 420), ImGuiCond_FirstUseEver);
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	if (ImGui::Begin("Main viewport", NULL, window_flags))
	{
		ImVec2 window_size = ImGui::GetWindowSize();
		
		// Render mainframe
		effect_tool->ResizeViewport(window_size.x, window_size.y);
		effect_tool->CoreRender();
		ComPtr<ID3D11ShaderResourceView> main_rtv = effect_tool->render_target_->srv_.Get();
		ImGui::Image((void*)main_rtv.Get(), window_size);
		
	}
	ImGui::End();
	ImGui::PopStyleVar();
}