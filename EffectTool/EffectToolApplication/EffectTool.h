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
				XMFLOAT2 size_min, XMFLOAT2 size_max, XMFLOAT2 lifetime_minmax, W_STR emitter_name);
	bool	NameExists(W_STR name);
private:
	std::map<W_STR, ParticleSystem*> emitters;

	ComPtr<ID3D11Buffer> gs_cbuffer_per_frame_;
	CdPerFrame gs_cdata_per_frame_;
};

