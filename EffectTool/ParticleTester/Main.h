#pragma once
#include "GameCore.h"
#include "Camera.h"
#include "ParticleSystem.h"
#include "d3dx11Effect.h"

class Main : public GameCore
{
private:
	struct CdPerFrame
	{
		XMFLOAT3 eye_pos;
	};

public:
	bool	Init()		override;
	bool	Frame()		override;
	bool	Render()	override;
	bool	Release()	override;

private:
	Object*	box_	=	nullptr;
	ComPtr<ID3D11Buffer> gs_cbuffer_per_frame_;
	CdPerFrame gs_cdata_per_frame_;
};

