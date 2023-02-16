#pragma once
#include "Std.h"
#include "Object.h"
#include "Camera.h"
#include "DxState.h"
#define randstep(fMin,fMax) (fMin+((float)fMax-(float)fMin)*rand()/(float)RAND_MAX)

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
		tex_coord = t;
		tex_idx = t_idx;
	};
};

struct CdPerParticleSystem
{
	int			axis_lock_type;
	BOOL		is_alpha_tested;
	BOOL		is_alpha_blended;
	BOOL		is_uv_animated;

	XMFLOAT2	sprite_dimension;
	XMFLOAT2	temp;

	CdPerParticleSystem()
	{
		axis_lock_type = 0;
		is_alpha_tested = false;
		is_alpha_blended = false;
		is_uv_animated = true;
		sprite_dimension = { 1,1 };
		temp = { 0,0 };
	}
};

struct Particle
{
	bool		active;
	XMFLOAT3	position;
	XMFLOAT3	velocity;
	XMFLOAT4	color;
	XMFLOAT2	size;
	float		timer;
	float		anim_timer;
	float		lifetime;

	Particle()
	{
		active = true;
		position = { 0.0f, 0.0f, 0.0f };
		velocity = { 0.0f, 1.0f, 0.0f };
		color = { 1,1,1, 1.0f };
		size = { 1,1 };
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
	//void	CreateVertexData();
	void	BuildVertexBuffer();
	void	BuildConstantBuffer();
	void	BuildInputLayout();
	void	EnhanceParticles();

public:
	bool	Frame();
	bool	PostRender();
	bool	Create(ID3D11Device* device, ID3D11DeviceContext* context);
	void	UpdateConstantBuffer();
	
	void	SetUVAnimation(W_STR sprite_sheet_path, float num_rows = 1, float num_cols = 1);
	void	SetMultiTexAnimation(std::vector<W_STR>& filenames);

	void	SetAlphaTesting(bool is_alpha_tested);
	void	SetAlphaBlending(bool is_alpha_blended);

	void	EmitParticles();
	void	SetEmitterProperties(float emit_range, int emit_number);
	void	SetLifetimeOffset(float min_lifetime, float max_lifetime);
public:
	std::vector<Particle>		particles_;
	int			num_textures;
	XMFLOAT3	emitter_pos_ = { 0,0,0 };

private:
	int		emit_num_particles = 10;
	float	emit_range = 1.0f;
	float	emit_timer = 0.0f;
	bool	is_uv_animated_;
	float	anim_offset = 0.3f;
	ComPtr<ID3D11Buffer>		gs_cbuffer_;
	

	CdPerParticleSystem			cd_per_system_;
	std::vector<ParticleVertex> particle_vertices_;
	
	XMFLOAT3	initial_pos_offset_min_;
	XMFLOAT3	initial_pos_offset_max_;
	XMFLOAT2	initial_size_offset_min_;
	XMFLOAT2	initial_size_offset_max_;
	XMFLOAT2	lifetime_offset = { -1,-1 };

	// not implemented
	bool		enabled_;
	
	bool		gravity_enabled_;
};

