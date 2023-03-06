#pragma once
#include "GameCore.h"
#include "Camera.h"
#include "ParticleSystem.h"
#include "RenderTarget.h"

class EffectTool : public GameCore
{
public:
	bool	Init()		override;
	bool	Frame()		override;
	bool	Render()	override;
	bool	Release()	override;
	
	void	ResizeViewport(int ps_idx, float width, float height);
	void	SetRenderStates();
	void	CreateParticleSystem();
	void	SetPSWindowState(int ps_idx, bool state);

	void	SetEmitterTexture(W_STR emitter_name, W_STR tex_path,
		int num_rows, int num_cols);
	void SetEmitterTexture(W_STR emitter_name,
		std::vector<W_STR>& tex_paths);
	void SetEmitterTexture(W_STR emitter_name, W_STR tex_path);

	void	SetSpawnRate(W_STR emitter_name, float spawn_rate);
	void	UpdateSizeOffset(W_STR emitter_name, XMFLOAT2 size_min, XMFLOAT2 size_max);

	bool	LoadParticleSystemFromFile(W_STR filename);
	//bool	SaveParticleSystemToFile(std::vector<ParticleEmitter>& particle_system, W_STR filename);

	ID3D11ShaderResourceView*	GetRenderedTexture(int ps_idx);
	int		GetNumEmittersInPS(int ps_idx);

public:
	bool	depth_write_enabled_	= false;
	bool	depth_compared_			= true;
	bool	alpha_tested_			= false;
	bool	alpha_blended_			= true;
	bool	wireframe_enabled_		= false;
	bool	dualsource_blended_		= true;

private:
	std::vector<ParticleSystem*>	particle_systems_;
	std::vector<RenderTarget*>		render_targets_;
	std::vector<bool>				ps_window_is_active_;
};

