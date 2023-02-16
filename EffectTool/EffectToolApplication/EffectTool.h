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
private:
	ParticleSystem* tex_anim_ps_;
	//ParticleSystem* uv_anim_ps_;
	ComPtr<ID3D11Buffer> gs_cbuffer_per_frame_;
	CdPerFrame gs_cdata_per_frame_;
};

