#pragma once
#include "Object.h"
#include "DxState.h"

struct Particle
{
	XMFLOAT3	position;
	XMFLOAT3	velocity;
	XMFLOAT4	color;
	XMFLOAT2	size;
	float		anim_timer;
	float		lifetime;

	Particle()
	{
		position = { 0.0f, 0.0f, 0.0f };
		velocity = { 0.0f, 1.0f, 0.0f };
		color = { 1,1,1, 1.0f };
		size = { 1,1 };
		anim_timer = 0.0f;
		lifetime = 5.0f;
	};
};

struct ParticleVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
	XMFLOAT2 size;
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
		size = s;
		tex_coord = { 1,1 };
		tex_idx = t_idx;
	};
};

// TODO: to implement
struct CdPerParticleEmitter 
{
	int			axis_lock_type;
	BOOL		is_alpha_tested; 
	BOOL		is_alpha_blended;
	BOOL		is_uv_animated;

	XMFLOAT2	sprite_dimension;
	XMFLOAT2	temp;

	CdPerParticleEmitter()
	{
		axis_lock_type = 0;
		is_alpha_tested = false;
		is_alpha_blended = false;
		is_uv_animated = true;
		sprite_dimension = { 1,1 };
		temp = { 0,0 };
	}
};

class ParticleEmitter : public Object
{
public:
	bool	Init();
	bool	Release();

private:
	bool	PreRender();
	void	BuildVertexBuffer();
	void	BuildConstantBuffer();
	void	BuildInputLayout();
	void	EnhanceParticles();

public:
	bool	Frame();
	bool	Render();
	bool	PostRender();
	bool	Create(ID3D11Device* device, ID3D11DeviceContext* context);
	void	UpdateConstantBuffer();
	
	void	SetUVAnimation(W_STR sprite_sheet_path, int num_rows = 1, int num_cols = 1);
	void	SetMultiTexAnimation(std::vector<W_STR>& filenames);

	void	SetAlphaTesting(bool is_alpha_tested);
	void	SetAlphaBlending(bool is_alpha_blended);
	void	SetDualBlending(bool is_dual_blended);

	void	InitialiseParticle(Particle& p);
	void	EmitParticle();
	void	SetName(W_STR name);
	void	SetSpawnRate(float spawn_rate);
	void	SetLifetimeOffset(float min_lifetime, float max_lifetime);
	void	SetPosOffset(XMFLOAT3 pos_offset_min, XMFLOAT3 pos_offset_max);
	void	SetSizeOffset(XMFLOAT2 size_min, XMFLOAT2 size_max);
	void	SetEmitterPos(XMFLOAT3 pos);
	void	SetVelocity(XMFLOAT3 velocity_min, XMFLOAT3 velocity_max);
	void	SetAdditiveColor(BOOL use_random_color);
	void	Reset();
	void	ResetTexture();
	void	SetTexture(W_STR filepath);

public:
	std::vector<Particle>		particles_;
	int			num_requested_particles = 0;
	int			num_textures;
	XMFLOAT3	emitter_pos_ = { 0,0,0 };
	int num_tex_rows_ = 1;
	int num_tex_cols_ = 1;
	int		num_alive_particles = 0;

private:
	W_STR	name_;
	XMFLOAT2 tex_coords[4];
	float	spawn_rate_;
	float	emit_interval_;
	
	float	emit_timer = 0.0f;
	bool	is_uv_animated_;
	float	anim_offset = 0.1f;
	bool	use_random_color_;
	
	bool	dual_blended_;
	bool	alpha_tested_;
	bool	alpha_blended_;
	
	ComPtr<ID3D11Buffer>		gs_cbuffer_;
	CdPerParticleEmitter		cd_per_system_;
	std::vector<ParticleVertex> particle_vertices_;
	std::vector<ParticleVertex> init_particle_vertices_;
	
	XMFLOAT3	initial_pos_offset_min_;
	XMFLOAT3	initial_pos_offset_max_;
	XMFLOAT2	initial_size_offset_min_;
	XMFLOAT2	initial_size_offset_max_;
	XMFLOAT2	lifetime_offset;
	XMFLOAT3	initial_velocity_min;
	XMFLOAT3	initial_velocity_max;

	// not implemented
	bool		enabled_;
	
	bool		gravity_enabled_;
};

