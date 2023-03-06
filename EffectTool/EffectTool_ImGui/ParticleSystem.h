#pragma once
#include "JsonHelper.h"
#include "ParticleEmitter.h"
#include "Camera.h"

struct VSConstantDataPerSystem
{
	XMFLOAT4X4  billboard;
	XMFLOAT4X4	view;
	XMFLOAT4X4	proj;
};

class ParticleSystem
{
public:
	void	SetDeviceContext(ID3D11Device* device, ID3D11DeviceContext* context);
	void	SetCamera(Camera* cam);
	void	SetCameraLens(float nearZ, float farZ, float fov, float aspect_ratio);
	
	int		GetNumEmitters();

	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	void	AddDefaultEmitter();

	static	ParticleSystem* LoadFromFile(ID3D11Device* device, W_STR filepath);

private:
	ComPtr<ID3D11Device>			device_;
	ComPtr<ID3D11DeviceContext>		device_context_;

	ComPtr<ID3D11Buffer>			gs_cbuffer_per_ps_;
	VSConstantDataPerSystem			gs_cdata_per_ps_;

	Camera* cam_;
	//TODO : active and inactive emitters

	std::vector<ParticleEmitter*>	emitters_;

public:
	W_STR name_;
};

