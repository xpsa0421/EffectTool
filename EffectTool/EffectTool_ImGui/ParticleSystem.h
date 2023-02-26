#pragma once
#include "JsonHelper.h"
#include "ParticleEmitter.h"

struct CdPerParticleSystem
{
	// TODO: Shared data for all emitters
};

class ParticleSystem
{
public:
	void	SetDeviceContext(ID3D11Device* device, ID3D11DeviceContext* context);
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	void	AddDefaultEmitter();

	static	ParticleSystem* LoadFromFile(ID3D11Device* device, W_STR filepath);

private:
	ComPtr<ID3D11Device>			device_;
	ComPtr<ID3D11DeviceContext>		device_context_;

public:
	std::vector<ParticleEmitter*>	emitters_;
};

