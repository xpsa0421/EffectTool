#pragma once
#include "Object.h"
#include "Camera.h"

struct CdPerParticleSystem
{
	XMFLOAT4X4	view_proj;
};

struct Particle
{
	XMFLOAT3	position;
	XMFLOAT3	velocity;
	XMFLOAT4	color;
	float		timer;
	float		lifetime;

	Particle()
	{
		position = { 0.0f, 0.0f, 0.0f };
		velocity = { 0.0f, 1.0f, 0.0f };
		color = { 0.7f, 0.3f, 0.5f, 1.0f };
		timer = 0.0f;
		lifetime = 5.0f;
	}
};

class ParticleSystem : public Object
{
public:
	bool	Init();
	bool	Release();

private:
	bool	PreRender();
	void	CreateVertexData();
	void	BuildConstantBuffer();

public:
	bool	Frame();
	bool	Create(ID3D11Device* device, ID3D11DeviceContext* context);
	void	UpdateConstantBuffer();

private:
	ComPtr<ID3D11Buffer>	gs_cbuffer_per_system;
	std::vector<Particle>	particles_;

	bool		enabled_;
	XMFLOAT3	initial_pos_offset_min_;
	XMFLOAT3	initial_pos_offset_max_;
	bool		gravity_enabled_;
};

