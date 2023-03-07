#include "ImGuiRenderer.h"

void ImGuiRenderer::Init(EffectTool* effect_tool)
{
	effect_tool_ = effect_tool;
	open_ps_options_popup_ = true;
	open_new_ps_popup_ = false;
	z_writing_ = false;
	z_comparing_ = true;
	wireframe_ = false;
}

void ImGuiRenderer::CreateMainMenu()
{
	// Main menu bar
	if (ImGui::BeginMainMenuBar())
	{
		// File menu
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New", "Ctrl+N"))
			{
				ps_name_[0] = '\0';
				open_new_ps_popup_ = true;
			}
			if (ImGui::MenuItem("Open", "Ctrl+O")) {}
			if (ImGui::MenuItem("Save", "Ctrl+S")) {}
			if (ImGui::MenuItem("Save As..")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Quit", "Alt+F4"))
			{
				effect_tool_->game_active_ = false;
			}
			ImGui::EndMenu();
		}

		// Options menu
		if (ImGui::BeginMenu("Options"))
		{
			ImGui::MenuItem("Render options", NULL, false, false);
			ImGui::Checkbox("Z-buffer writing", &z_writing_);
			ImGui::Checkbox("Z-buffer comparing", &z_comparing_);
			ImGui::Separator();
			ImGui::Checkbox("Wireframe", &wireframe_);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

ImGuiID ImGuiRenderer::CreateMasterDockSpace()
{
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDocking;
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

	ImGui::Begin("Master Window", nullptr, window_flags);
	ImGui::PopStyleVar(3);
	ImGuiID master_dockspace_id = ImGui::GetID("Master DockSpace");
	ImGui::DockSpace(master_dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	ImGui::End();

	return master_dockspace_id;
}

void ImGuiRenderer::CreateParticleWindow(int ps_idx)
{
	std::string ps_name = wtm(effect_tool_->GetPSName(ps_idx));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove;

	bool window_active = ImGui::Begin((ps_name + "##").c_str() + ps_idx, NULL, window_flags);
	effect_tool_->SetPSWindowState(ps_idx, window_active);

	ImGuiDockNodeFlags dock_node_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	if (!window_active)
		dock_node_flags |= ImGuiDockNodeFlags_KeepAliveOnly;

	ImVec2 emitter_content_size = ImGui::GetContentRegionAvail();
	ImGuiID main_dockspace = ImGui::GetID("PS DockSpace##" + ps_idx);
	ImGui::DockSpace(main_dockspace, ImVec2(0.0f, 0.0f), dock_node_flags);

	static auto first_time = true;
	if (first_time)
	{
		first_time = false;

		ImGui::DockBuilderRemoveNode(main_dockspace);
		ImGui::DockBuilderAddNode(main_dockspace, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(main_dockspace, emitter_content_size);

		ImGuiID dock_id_left, dock_id_upLeft, dock_id_downLeft;
		ImGuiID dock_id_right, dock_id_upRight, dock_id_downRight;
		ImGui::DockBuilderSplitNode(main_dockspace, ImGuiDir_Left, 0.37f, &dock_id_left, &dock_id_right);
		ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Up, 0.530f, &dock_id_upLeft, &dock_id_downLeft);
		ImGui::DockBuilderSplitNode(dock_id_right, ImGuiDir_Up, 0.60f, &dock_id_upRight, &dock_id_downRight);

		ImGui::DockBuilderDockWindow("Preview##" + ps_idx, dock_id_upLeft);
		ImGui::DockBuilderDockWindow("Properties##" + ps_idx, dock_id_downLeft);
		ImGui::DockBuilderDockWindow("Emitters##" + ps_idx, dock_id_upRight);
		ImGui::DockBuilderDockWindow("Curve Editor##" + ps_idx, dock_id_downRight);

		ImGui::DockBuilderFinish(main_dockspace);
	}
	ImGui::End();

	window_flags = ImGuiWindowFlags_NoCollapse;
	if (ImGui::Begin("Preview##" + ps_idx), NULL, window_flags)
	{
		if (window_active)
		{
			// Render mainframe
			ImVec2 content_size = ImGui::GetContentRegionAvail();
			effect_tool_->ResizeViewport(ps_idx, content_size.x, content_size.y);
			ImGui::Image((void*)effect_tool_->GetRenderedTexture(ps_idx), content_size);
		}
		ImGui::End();
	}

	window_flags = ImGuiWindowFlags_HorizontalScrollbar;
	int num_emitters = effect_tool_->GetNumEmittersInPS(ps_idx);
	ImGui::SetNextWindowContentSize(ImVec2(num_emitters * 200 + 5, 0));

	static bool display_emitters = true;
	if (ImGui::Begin("Emitters##" + ps_idx, &display_emitters, ImGuiWindowFlags_HorizontalScrollbar))
	{
		int numProps = 5;
		static std::vector<int> selected(numProps, -1);
		const ImVec2 layoutSize{ 200, 0 };
		const ImGuiWindowFlags layoutFlags = ImGuiWindowFlags_NoMove;

		for (int emitter_idx = 0; emitter_idx < num_emitters; emitter_idx++) {
			std::string layoutName{ std::string("Emitter ") + std::to_string(emitter_idx) + "##" + std::to_string(ps_idx) };
			ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.f);

			if (ImGui::BeginChild(layoutName.c_str(), layoutSize, true)) {
				ImGui::Text("Emitter %i", emitter_idx);
				for (int prop_idx = 0; prop_idx < numProps; prop_idx++)
				{
					std::string name = "Property " + std::to_string(prop_idx) + "##" + std::to_string(ps_idx);
					if (ImGui::Selectable(name.c_str(), selected[emitter_idx] == prop_idx))
						selected[emitter_idx] = prop_idx;
				}
			}
			ImGui::PopStyleVar();
			ImGui::EndChild();
			ImGui::SameLine(0, 0);
		}
	}
	ImGui::End();

	window_flags = ImGuiWindowFlags_NoCollapse;
	if (ImGui::Begin("Properties##" + ps_idx))
	{

	}ImGui::End();

	if (ImGui::Begin("Curve Editor##" + ps_idx))
	{

	}
	ImGui::End();
	ImGui::PopStyleVar();
}

void ImGuiRenderer::CreateNoParticlePopUp()
{
	// Display popup modal when no particle system is present
	if (open_ps_options_popup_) {
		ImGui::OpenPopup("Oops!");
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 10));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 15));

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
	if (ImGui::BeginPopupModal("Oops!", NULL, window_flags))
	{
		ImGui::Text("No particle systems to display.");

		if (ImGui::Button("New", ImVec2(80, 0)))
		{
			ps_name_[0] = '\0';
			//strcpy(ps_name, "");

			open_ps_options_popup_ = false;
			open_new_ps_popup_ = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Load", ImVec2(80, 0)))
		{
			open_ps_options_popup_ = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Quit", ImVec2(80, 0)))
		{
			open_ps_options_popup_ = false;
			effect_tool_->game_active_ = false;
		}

		if (!open_ps_options_popup_)
			ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar(2);
}

void ImGuiRenderer::CreateNewParticlePopUp()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 10));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 15));

	if (open_new_ps_popup_) ImGui::OpenPopup("Create New Particle System?");
	ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Create New Particle System?", NULL,
		ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
	{
		ImGui::Text("Name:");
		ImGui::SameLine();
		if (!ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
			ImGui::SetKeyboardFocusHere(0);
		ImGui::InputText("##", ps_name_, IM_ARRAYSIZE(ps_name_));
		ImGui::SameLine();
		if (ImGui::Button("Create", ImVec2(70, 0)))
		{
			open_new_ps_popup_ = false;
			effect_tool_->CreateParticleSystem(mtw(std::string(ps_name_)));
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::PopStyleVar(2);
}

void ImGuiRenderer::Render()
{
	// Main menu
	CreateMainMenu();

	// Main docking window
	ImGuiID master_dockspace_id = CreateMasterDockSpace();

	// Particle systems viewer
	int num_particle_systems = effect_tool_->GetNumParticleSystems();
	for (int ps_idx = 0; ps_idx < num_particle_systems; ps_idx++)
	{
		ImGui::SetNextWindowDockID(master_dockspace_id, ImGuiCond_Once);
		CreateParticleWindow(ps_idx);
	}

	CreateNoParticlePopUp();
	CreateNewParticlePopUp();	
}