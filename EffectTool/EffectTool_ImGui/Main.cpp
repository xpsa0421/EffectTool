#include "Std.h"
#include "EffectTool.h"
#include "imgui.h"
#include "..\ImGui\imgui_internal.h"

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

			// Render
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
	// Main menu bar
	if (ImGui::BeginMainMenuBar())
	{
		ImGui::MenuItem("Save");
		ImGui::MenuItem("Load");
		ImGui::EndMainMenuBar();
	}

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	if (ImGui::Begin("DockSpace Demo", NULL, window_flags))
	{
		ImGuiID main_dockspace = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(main_dockspace, ImVec2(0.0f, 0.0f), dockspace_flags);

		static auto first_time = true;
		if (first_time)
		{
			first_time = false;
			ImGui::DockBuilderRemoveNode(main_dockspace);
			ImGui::DockBuilderAddNode(main_dockspace, ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(main_dockspace, ImGui::GetMainViewport()->WorkSize);
			
			ImGuiID dock_id_left, dock_id_upLeft, dock_id_downLeft;
			ImGuiID dock_id_right, dock_id_upRight, dock_id_downRight;
			ImGui::DockBuilderSplitNode(main_dockspace, ImGuiDir_Left, 0.50f, &dock_id_left, &dock_id_right);
			ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Up, 0.40f, &dock_id_upLeft, &dock_id_downLeft);
			ImGui::DockBuilderSplitNode(dock_id_right, ImGuiDir_Up, 0.60f, &dock_id_upRight, &dock_id_downRight);

			ImGui::DockBuilderDockWindow("Preview", dock_id_upLeft);
			ImGui::DockBuilderDockWindow("Properties", dock_id_downLeft);
			ImGui::DockBuilderDockWindow("Emitters", dock_id_upRight);
			ImGui::DockBuilderDockWindow("Curve Editor", dock_id_downRight);

			ImGui::DockBuilderFinish(main_dockspace);
		}
	}
	ImGui::End();
	ImGui::PopStyleVar();


	window_flags = ImGuiWindowFlags_NoCollapse;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	if (ImGui::Begin("Preview"))// , NULL, window_flags))
	{
		ImVec2 content_size = ImGui::GetContentRegionAvail();

		// Render mainframe
		effect_tool->ResizeViewport(content_size.x, content_size.y);
		effect_tool->CoreRender();
		ImGui::Image((void*)effect_tool->GetRenderedTexture(), content_size);
	}
	ImGui::End();
	ImGui::PopStyleVar();


	if (ImGui::Begin("Properties"))
	{

	}
	ImGui::End();


	if (ImGui::Begin("Emitters"))
	{

	}
	ImGui::End();

	if (ImGui::Begin("Curve Editor"))
	{

	}
	ImGui::End();
}