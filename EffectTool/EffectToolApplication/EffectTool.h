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
	void	GenEmitterFromMultipleTex(std::vector<W_STR> tex_names,
				int num_particles, float emit_cycle, XMFLOAT3 emitter_pos, XMFLOAT3 pos_offset_min, XMFLOAT3 pos_offset_max,
				XMFLOAT2 size_min, XMFLOAT2 size_max, XMFLOAT2 lifetime_minmax, 
				XMFLOAT3 velocity_min, XMFLOAT3 velocity_max, BOOL use_random_color, W_STR emitter_name);
	bool	NameExists(W_STR name);
	void	CreateDefaultEmitter();
	void	SetEmitterTexture(W_STR emitter_name, W_STR tex_path,
		int num_rows, int num_cols);
	void SetEmitterTexture(W_STR emitter_name,
		std::vector<W_STR>& tex_paths);
	void SetEmitterTexture(W_STR emitter_name, W_STR tex_path);
	void	SetSpawnRate(W_STR emitter_name, float spawn_rate);
public:
	bool	depth_write_enabled_	= false;
	bool	depth_compared_			= true;
	bool	alpha_tested_			= false;
	bool	alpha_blended_			= true;
	bool	wireframe_enabled_		= false;

private:
	std::map<W_STR, ParticleSystem*> emitters;

	ComPtr<ID3D11Buffer> gs_cbuffer_per_frame_;
	CdPerFrame gs_cdata_per_frame_;
};

