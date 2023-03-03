#define _CRT_SECURE_NO_WARNINGS
#include "Std.h"
#include "EffectTool.h"
#include "imgui.h"
#include "..\ImGui\imgui_internal.h"

//------------------------------------------------------------
// Forward declarations of imgui render functions
void ImGuiRender(EffectTool* effect_tool);
void CreateParticleViewer(EffectTool* effect_tool, int particle_idx);
//------------------------------------------------------------


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
			effect_tool.CoreRender();

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
		// File menu
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New")) {}
			if (ImGui::MenuItem("Open", "Ctrl+O")) {}
			if (ImGui::MenuItem("Save", "Ctrl+S")) {}
			if (ImGui::MenuItem("Save As..")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Quit", "Alt+F4"))
			{
				effect_tool->game_active_ = false;
			}
			ImGui::EndMenu();
		}

		// Options menu
		if (ImGui::BeginMenu("Options"))
		{
			ImGui::MenuItem("Render options", NULL, false, false);
			static bool alpha_testing = true;
			static bool alpha_blending = true;
			ImGui::Checkbox("Alpha testing", &alpha_testing);
			ImGui::Checkbox("Alpha blending", &alpha_blending);
			ImGui::Separator();
			static bool z_writing = false;
			static bool z_comparing = true;
			ImGui::Checkbox("Z-buffer writing", &z_writing);
			ImGui::Checkbox("Z-buffer comparing", &z_comparing);
			ImGui::Separator();
			static bool wireframe = false;
			ImGui::Checkbox("Wireframe", &wireframe);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	/*const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGuiWindowFlags host_window_flags = 0;
	host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
	host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("HostWindow", NULL, host_window_flags);
	ImGui::PopStyleVar(3);
	ImGuiID dockspace_id = ImGui::GetID("HostDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::End();*/


	//ImGui::Begin("HostWindow");
		/*ImGuiID host_dockspace_id = ImGui::GetID("HostDockspace"); 
		ImGui::DockSpace(host_dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoResize);*/
		ImGuiID host_dockspace_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
	//ImGui::End();

	ImGui::SetNextWindowDockID(host_dockspace_id, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Window1"), ImGuiWindowFlags_NoBackground) { ImGui::Text("window 1"); }
	ImGui::End();

	ImGui::SetNextWindowDockID(host_dockspace_id, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Window2")) { ImGui::Text("window 2"); }
	ImGui::End();

	//CreateParticleViewer(effect_tool, 0);
}

static void CreateParticleViewer(EffectTool* effect_tool, int particle_idx)
{
	//const ImGuiViewport* viewport = ImGui::GetMainViewport();
	//ImGui::SetNextWindowPos(viewport->WorkPos);
	//ImGui::SetNextWindowSize(viewport->WorkSize);
	//ImGui::SetNextWindowViewport(viewport->ID);
	//ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize /*| ImGuiWindowFlags_NoMove*/;
	//window_flags |= /*ImGuiWindowFlags_NoBringToFrontOnFocus | */ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDocking;
	//ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	//if (ImGui::Begin("Particle##" + particle_idx, NULL, window_flags))
	//{
	//	ImGuiID main_dockspace = ImGui::GetID("MyDockSpace##" + particle_idx);
	//	ImGui::DockSpace(main_dockspace, ImVec2(0.0f, 0.0f), dockspace_flags);

	//	static auto first_time = true;
	//	if (first_time)
	//	{
	//		first_time = false;

	//		ImGui::DockBuilderRemoveNode(main_dockspace);
	//		ImGui::DockBuilderAddNode(main_dockspace, ImGuiDockNodeFlags_DockSpace);
	//		ImGui::DockBuilderSetNodeSize(main_dockspace, ImGui::GetMainViewport()->WorkSize);

	//		ImGuiID dock_id_left, dock_id_upLeft, dock_id_downLeft;
	//		ImGuiID dock_id_right, dock_id_upRight, dock_id_downRight;
	//		ImGui::DockBuilderSplitNode(main_dockspace, ImGuiDir_Left, 0.37f, &dock_id_left, &dock_id_right);
	//		ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Up, 0.530f, &dock_id_upLeft, &dock_id_downLeft);
	//		ImGui::DockBuilderSplitNode(dock_id_right, ImGuiDir_Up, 0.60f, &dock_id_upRight, &dock_id_downRight);

	//		ImGui::DockBuilderDockWindow("Preview##" + particle_idx, dock_id_upLeft);
	//		ImGui::DockBuilderDockWindow("Properties##" + particle_idx, dock_id_downLeft);
	//		ImGui::DockBuilderDockWindow("Emitters##" + particle_idx, dock_id_upRight);
	//		ImGui::DockBuilderDockWindow("Curve Editor##" + particle_idx, dock_id_downRight);

	//		ImGui::DockBuilderFinish(main_dockspace);
	//	}
	//}
	//ImGui::End();

	//window_flags = ImGuiWindowFlags_NoCollapse;

	//if (ImGui::Begin("Preview##" + particle_idx), NULL, window_flags)
	//{
	//	ImVec2 content_size = ImGui::GetContentRegionAvail();

	//	// Render mainframe
	//	effect_tool->ResizeViewport(content_size.x, content_size.y);
	//	effect_tool->CoreRender();
	//	ImGui::Image((void*)effect_tool->GetRenderedTexture(), content_size);

	//	ImGui::End();
	//}

	//window_flags = ImGuiWindowFlags_HorizontalScrollbar;
	//const int numEmitters = 4;
	//ImGui::SetNextWindowContentSize(ImVec2(numEmitters * 200 + 5, 0));

	//static bool display_emitters = true;
	//if (ImGui::Begin("Emitters##" + particle_idx, &display_emitters, ImGuiWindowFlags_HorizontalScrollbar))
	//{
	//	int numProps = 5;
	//	static std::vector<int> selected(numProps, -1);
	//	const ImVec2 layoutSize{ 200, 0 };
	//	const ImGuiWindowFlags layoutFlags = ImGuiWindowFlags_NoMove;

	//	for (int emitter_idx = 0; emitter_idx < numEmitters; emitter_idx++) {
	//		std::string layoutName{ std::string("Emitter##" + particle_idx) + std::to_string(emitter_idx) };
	//		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.f);

	//		if (ImGui::BeginChild(layoutName.c_str(), layoutSize, true)) {
	//			ImGui::Text("Emitter %i", emitter_idx);
	//			for (int prop_idx = 0; prop_idx < numProps; prop_idx++)
	//			{
	//				std::string name = "Property##" + particle_idx + std::to_string(prop_idx);
	//				if (ImGui::Selectable(name.c_str(), selected[emitter_idx] == prop_idx))
	//					selected[emitter_idx] = prop_idx;
	//			}
	//		}

	//		ImGui::PopStyleVar();
	//		ImGui::EndChild();
	//		ImGui::SameLine(0, 0);
	//	}
	//}
	//ImGui::End();

	//window_flags &= !ImGuiWindowFlags_HorizontalScrollbar;
	//if (ImGui::Begin("Properties##" + particle_idx))
	//{

	//}ImGui::End();

	//if (ImGui::Begin("Curve Editor##" + particle_idx))
	//{

	//}
	//ImGui::End();
	//ImGui::PopStyleVar();
}