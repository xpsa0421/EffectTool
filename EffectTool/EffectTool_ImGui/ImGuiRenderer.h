#pragma once
#include "Std.h"
#include "imgui.h"
#include "..\ImGui\imgui_internal.h"
#include "EffectTool.h"

class ImGuiRenderer
{
private:
	void		CreateMainMenu();
	ImGuiID		CreateMasterDockSpace();
	void		CreateParticleWindow(int ps_idx);
	void		CreateNoParticlePopUp();
	void		CreateNewParticlePopUp();

public:
	void Init(EffectTool* effect_tool);
	void Render();

private:
	EffectTool* effect_tool_;

	bool	open_new_ps_popup_;
	bool	open_ps_options_popup_;

	bool	z_writing_;
	bool	z_comparing_;
	bool	wireframe_;

	char	ps_name_[128];
};

