#pragma once
#include "GameCore.h"
#include "Camera.h"
#include "ParticleSystem.h"

struct CdPerFrame
{
	XMFLOAT4X4	billboard;
	XMFLOAT4X4	view;
	XMFLOAT4X4	proj;
};

class EffectTool : public GameCore
{
public:
	bool	Init()		override;
	bool	Frame()		override;
	bool	Render()	override;
	bool	Release()	override;
	HRESULT CreateDXResource() override;
	
	void	AddDefaultParticleSystem();


	void	SetEmitterTexture(W_STR emitter_name, W_STR tex_path,
		int num_rows, int num_cols);
	void SetEmitterTexture(W_STR emitter_name,
		std::vector<W_STR>& tex_paths);
	void SetEmitterTexture(W_STR emitter_name, W_STR tex_path);

	void	SetSpawnRate(W_STR emitter_name, float spawn_rate);
	void	UpdateSizeOffset(W_STR emitter_name, XMFLOAT2 size_min, XMFLOAT2 size_max);

	bool	LoadParticleSystemFromFile(W_STR filename);
	//bool	SaveParticleSystemToFile(std::vector<ParticleEmitter>& particle_system, W_STR filename);
	

public:
	bool	depth_write_enabled_	= false;
	bool	depth_compared_			= true;
	bool	alpha_tested_			= false;
	bool	alpha_blended_			= true;
	bool	wireframe_enabled_		= false;
	bool	dualsource_blended_		= true;

private:
	std::vector<ParticleSystem*> particle_systems;

	ComPtr<ID3D11Buffer> gs_cbuffer_per_frame_;
	CdPerFrame gs_cdata_per_frame_;
};

