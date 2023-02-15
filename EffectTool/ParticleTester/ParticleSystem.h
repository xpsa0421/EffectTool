#pragma once
#include "Object.h"
#include "Camera.h"
#include "DxState.h"
#define randstep(fMin,fMax) (fMin+((float)fMax-(float)fMin)*rand()/(float)RAND_MAX)

struct ParticleVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
	XMFLOAT2 initial_size;
	XMFLOAT2 tex_coord;
	UINT	 tex_idx;
	
	ParticleVertex(XMFLOAT3 p = XMFLOAT3(0, 0, 0),
		XMFLOAT4 c = XMFLOAT4(0, 0, 0, 0),
		XMFLOAT2 s = XMFLOAT2(0, 0),
		XMFLOAT2 t = XMFLOAT2(0, 0),
		UINT t_idx = 0)
	{
		pos = p;
		color = c;
		initial_size = s;
		tex_coord = t;
		tex_idx = t_idx;
	};
};

struct CdPerParticleSystem
{
	UINT		axis_lock_type;
	XMFLOAT3	temp;
};

struct Particle
{
	XMFLOAT3	position;
	XMFLOAT3	velocity;
	XMFLOAT4	color;
	float		timer;
	float		anim_timer;
	float		lifetime;

	Particle()
	{
		position = { 0.0f, 0.0f, 0.0f };
		velocity = { 0.0f, 1.0f, 0.0f };
		color = { 0.7f, 0.3f, 0.5f, 1.0f };
		timer = 0.0f;
		anim_timer = 0.0f;
		lifetime = 5.0f;
	};
};

class ParticleSystem : public Object
{
public:
	bool	Init();
	bool	Release();

private:
	bool	PreRender();
	void	CreateVertexData();
	void	BuildVertexBuffer();
	void	BuildConstantBuffer();
	void	BuildInputLayout();

public:
	bool	Frame();
	bool	PostRender();
	bool	Create(ID3D11Device* device, ID3D11DeviceContext* context);
	void	UpdateConstantBuffer();
	void	AddTextureToArray();

public:
	int		num_particles_ = 0;
	std::vector<Particle>		particles_;
	int		num_textures;

private:
	bool	is_uv_animated_;
	float	anim_offset = 0.3f;
	ComPtr<ID3D11Buffer>		gs_cbuffer_;
	

	CdPerParticleSystem			gs_cdata_;
	std::vector<ParticleVertex> particle_vertices_;
	

	// not implemented
	bool		enabled_;
	XMFLOAT3	initial_pos_offset_min_;
	XMFLOAT3	initial_pos_offset_max_;
	bool		gravity_enabled_;
};

