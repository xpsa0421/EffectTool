#include "ParticleEmitter.h"
#define MAX_RENDER_COUNT 1000

bool ParticleEmitter::Init()
{	 
	tex_coords[0] = { 0.0, 0.0 };
	tex_coords[1] = { 1.0, 0.0 };
	tex_coords[2] = { 0.0, 0.1 };
	tex_coords[3] = { 1.0, 1.0 };

	dual_blended_ = false;
	alpha_tested_ = false;
	alpha_blended_ = true;

	cd_per_system_.axis_lock_type = 0;
	gs_cbuffer_ = nullptr;
	is_uv_animated_ = true; // TO CHANGE
	initial_pos_offset_min_ = { 0,0,0 };
	initial_pos_offset_max_ = { 0,0,0 };
	initial_size_offset_min_ = {0.5f, 0.5f};
	initial_size_offset_max_ = { 3,3 };
	initial_velocity_min = { 0,0,0 };
	initial_velocity_max = { 0,0,0 };
	lifetime_offset = { 0,0 };
	Object::Init();

	SetVertexShader(L"../../data/shader/VertexShader.hlsl", L"main");
	SetGeometryShader(L"../../data/shader/GeometryShader.hlsl", L"main");

	return true;
}	 
	 
bool ParticleEmitter::Release()
{
	gs_cbuffer_ = nullptr;
	if (!texture_) texture_srv_->Release();
	
	return Object::Release();
}
	 
void ParticleEmitter::BuildVertexBuffer()
{
	// generate vertex buffer
	D3D11_BUFFER_DESC vertex_desc;
	ZeroMemory(&vertex_desc, sizeof(vertex_desc));
	//vertex_desc.Usage = D3D11_USAGE_DEFAULT;
	num_requested_particles = ceil(lifetime_offset.y * spawn_rate_);
	int num_particles = min(MAX_RENDER_COUNT, ceil(lifetime_offset.y * spawn_rate_));
	particle_vertices_.reserve(num_particles);
	particles_.reserve(num_particles);

	vertex_desc.ByteWidth = sizeof(ParticleVertex) * (num_particles + 2); //  to change 
	vertex_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//vertex_desc.CPUAccessFlags = 0;
	//vertex_desc.MiscFlags = 0;
	//vertex_desc.StructureByteStride = 0;

	HRESULT result = device_->CreateBuffer(&vertex_desc, nullptr, vertex_buffer_.GetAddressOf());
	#ifdef _DEBUG
	if (FAILED(result))
	{
		OutputDebugStringA("[ParticleEmitter] Failed to build a vertex buffer\n");
	}
	#endif // _DEBUG
}

void ParticleEmitter::BuildConstantBuffer()
{
	// generate per particle system constant buffer
	D3D11_BUFFER_DESC constant_desc;
	ZeroMemory(&constant_desc, sizeof(constant_desc));
	constant_desc.ByteWidth = sizeof(CdPerParticleEmitter) * 1;
	constant_desc.Usage = D3D11_USAGE_DEFAULT;
	constant_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA constant_sub_data;
	constant_sub_data.pSysMem = &cd_per_system_;

	HRESULT result = device_->CreateBuffer(&constant_desc, &constant_sub_data, gs_cbuffer_.GetAddressOf());
	#ifdef _DEBUG
	if (FAILED(result))
	{
		printf("[Particle System] Failed to build per system constant buffer\n");
	}
	#endif // _DEBUG

	UpdateConstantBuffer();
}

void ParticleEmitter::BuildInputLayout()
{
	_ASSERT(vertex_shader_);
	D3D11_INPUT_ELEMENT_DESC vertex_descs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXIDX", 0, DXGI_FORMAT_R32_UINT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	HRESULT result = device_->CreateInputLayout(
		vertex_descs,
		sizeof(vertex_descs) / sizeof(vertex_descs[0]),
		vertex_shader_->code_->GetBufferPointer(),
		vertex_shader_->code_->GetBufferSize(),
		input_layout_.GetAddressOf()
	);
#ifdef _DEBUG
	if (FAILED(result))
	{
		printf("[ParticleEmitter] Failed to build input layout\n");
	}
#endif // _DEBUG
}

void ParticleEmitter::EnhanceParticles()
{
	int index = 0;
	while (index < num_alive_particles)
	{
		Particle& p = particles_[index];
		p.lifetime -= g_delta_time;

		if (p.lifetime <= 0.0f)
		{
			std::swap(particles_[index], particles_[num_alive_particles - 1]);
			particle_vertices_[index].tex_idx = particle_vertices_[num_alive_particles - 1].tex_idx;
			num_alive_particles--;
		}
		else
		{
			// update texture
			p.anim_timer += g_delta_time;
			if (p.anim_timer > anim_offset)
			{
				particle_vertices_[index].tex_idx++;
				if (particle_vertices_[index].tex_idx == num_textures) particle_vertices_[index].tex_idx = 0;
				p.anim_timer -= anim_offset;
			}

			XMVECTOR displacement = g_delta_time * XMLoadFloat3(&p.velocity);
			XMStoreFloat3(&p.position, XMVectorAdd(XMLoadFloat3(&p.position), displacement));
			particle_vertices_[index].pos = p.position;
			particle_vertices_[index].color = p.color;
			particle_vertices_[index].size = p.size;

			index++;
		}
	}

	if (!particle_vertices_.empty())
	{
		device_context_->UpdateSubresource(vertex_buffer_.Get(), 0, 0,
			&particle_vertices_.at(0), 0, 0);
	}	
}

void ParticleEmitter::Reset()
{
	particle_vertices_.clear();
	particles_.clear();

	EmitParticle();
}

bool ParticleEmitter::Frame()
{
	if (spawn_rate_ == 0) return false;

	// emit new particle
	emit_timer += g_delta_time;
	if (num_alive_particles < MAX_RENDER_COUNT)
	{
		if (emit_timer > emit_interval_)
		{
			EmitParticle();
			emit_timer = 0;
		}
	}
	EnhanceParticles();
	UpdateConstantBuffer();

	return true;
}

bool ParticleEmitter::PreRender()
{
	dual_blended_ = true;
	alpha_tested_ = false;
	alpha_blended_ = true;
	// Set blend state and pixel shader according to blend options
	if (!dual_blended_)
	{
		DxState.ApplyBlendState(L"BS_alpha");
		SetPixelShader(L"../../data/shader/PixelShader.hlsl", L"NoDualAlphaBlend");
	}
	else
	{
		if (alpha_blended_)
			DxState.ApplyBlendState(L"BS_dual_source_blend");
		else
			DxState.ApplyBlendState(L"BS_dual_source_no_blend");

		if (alpha_tested_)
			SetPixelShader(L"../../data/shader/PixelShader.hlsl", L"AlphaTested");
		else
			SetPixelShader(L"../../data/shader/PixelShader.hlsl", L"AlphaNotTested");
	}

	UINT stride = sizeof(ParticleVertex);
	UINT offset = 0;

	device_context_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	device_context_->IASetVertexBuffers(0, 1, vertex_buffer_.GetAddressOf(), &stride, &offset);
	device_context_->IASetInputLayout(input_layout_.Get());
	device_context_->VSSetShader(vertex_shader_->shader_.Get(), NULL, 0);
	device_context_->GSSetShader(geo_shader_->shader_.Get(), NULL, 0);
	device_context_->GSSetConstantBuffers(1, 1, gs_cbuffer_.GetAddressOf());
	device_context_->PSSetShader(pixel_shader_->shader_.Get(), NULL, 0);
	if (texture_srv_) device_context_->PSSetShaderResources(0, 1, texture_srv_.GetAddressOf());

	return true;
}

bool ParticleEmitter::Render()
{
	if (num_alive_particles == 0) return false;
	else return Object::Render();
}

bool ParticleEmitter::PostRender()
{
	device_context_->Draw(num_alive_particles, 0);

	return true;
}

bool ParticleEmitter::Create(ID3D11Device* device, ID3D11DeviceContext* context)
{
	device_ = device;
	device_context_ = context;

	// generate buffers
	BuildConstantBuffer();
	BuildVertexBuffer();

	// generate input layout
	BuildInputLayout();

	return true;
}
	
void ParticleEmitter::UpdateConstantBuffer()
{
	device_context_->UpdateSubresource(gs_cbuffer_.Get(), 0, 0, &cd_per_system_, 0, 0);
}

void ParticleEmitter::ResetTexture()
{
	// reset variables
	if ((!texture_) && (texture_srv_))
	{
		texture_srv_->Release();
		texture_srv_ = nullptr;
	}
	texture_ = nullptr;

	//reset tex coordinates
	tex_coords[0] = { 0.0, 0.0 };
	tex_coords[1] = { float(1.0 / num_tex_cols_), 0.0 };
	tex_coords[2] = { 0.0, float(1.0 / num_tex_rows_) };
	tex_coords[3] = { float(1.0 / num_tex_cols_), float(1.0 / num_tex_rows_) };

	if (!particle_vertices_.empty())
	{
		for (ParticleVertex& p_v : particle_vertices_)
		{
			p_v.tex_idx = 0;
			p_v.tex_coord = { float(num_tex_rows_), float(num_tex_cols_) };
		}
	}
}

void ParticleEmitter::SetTexture(W_STR filepath)
{
	num_tex_cols_ = 1;
	num_tex_rows_ = 1;
	num_textures = 1;
	ResetTexture();
	Object::SetTexture(filepath);
}

void ParticleEmitter::SetMultiTexAnimation(std::vector<W_STR>& filenames)
{
	num_textures = filenames.size();
	num_tex_cols_ = 1;
	num_tex_rows_ = 1;
	// create shader resource view from texture 2d array
	ResetTexture();
	SetTextureSRV(CreateSRVFromMultipleImages(device_.Get(), device_context_.Get(), filenames));
	
}

void ParticleEmitter::SetUVAnimation(W_STR filename, int num_rows, int num_cols)
{
	num_textures = num_rows * num_cols;
	num_tex_cols_ = num_cols;
	num_tex_rows_ = num_rows;
	ResetTexture();
	SetTextureSRV(CreateSRVFromSpriteSheet(device_.Get(), device_context_.Get(), filename, num_rows, num_cols));
}

void ParticleEmitter::SetAlphaTesting(bool is_alpha_tested)
{
	cd_per_system_.is_alpha_tested = is_alpha_tested;
	alpha_tested_ = is_alpha_tested;

}

void ParticleEmitter::SetAlphaBlending(bool is_alpha_blended)
{
	cd_per_system_.is_alpha_tested = is_alpha_blended;
	alpha_blended_ = is_alpha_blended;
}

void ParticleEmitter::SetDualBlending(bool is_dual_blended)
{
	dual_blended_ = is_dual_blended;
}

void ParticleEmitter::SetLifetimeOffset(float min_lifetime, float max_lifetime)
{
	lifetime_offset = { min_lifetime, max_lifetime };
}

void ParticleEmitter::InitialiseParticle(Particle& p)
{
	p.position = { emitter_pos_.x + randstep(initial_pos_offset_min_.x, initial_pos_offset_max_.x),
					emitter_pos_.y + randstep(initial_pos_offset_min_.y, initial_pos_offset_max_.y),
					emitter_pos_.z + randstep(initial_pos_offset_min_.z, initial_pos_offset_max_.z) };
	p.size = { randstep(initial_size_offset_min_.x, initial_size_offset_max_.x),
				randstep(initial_size_offset_min_.y, initial_size_offset_max_.y) };
	p.lifetime = { randstep(lifetime_offset.x, lifetime_offset.y) };
	p.velocity = { randstep(initial_velocity_min.x, initial_velocity_max.x),
					randstep(initial_velocity_min.y, initial_velocity_max.y) ,
					randstep(initial_velocity_min.y, initial_velocity_max.y) };
	if (use_random_color_)
		p.color = { randstep(0,1),randstep(0,1),randstep(0,1),1 };
	else
		p.color = { 1,1,1,1 };
}

void ParticleEmitter::EmitParticle()
{
	// check if there is any unused particle
	if (particles_.size() > num_alive_particles)
	{
		// there is an unused particle
		// retrieve the first unused particle and reuse it
		Particle& p = particles_[num_alive_particles]; 
		InitialiseParticle(p);
		particle_vertices_[num_alive_particles].tex_idx = 0;
	}
	else
	{
		// there is no unused particle
		// create a new particle object
		Particle p;
		InitialiseParticle(p);
		particles_.push_back(p);

		ParticleVertex p_v;
		p_v.tex_coord = { float(num_tex_rows_), float(num_tex_cols_) }; // TODO: pass to constant buffer instead
		particle_vertices_.push_back(p_v);
	}
	num_alive_particles++;
}

void ParticleEmitter::SetName(W_STR name)
{
	name_ = name;
}

void ParticleEmitter::SetSpawnRate(float spawn_rate)
{
	spawn_rate_ = spawn_rate;
	emit_interval_ = 1 / spawn_rate;

	if (spawn_rate == 0)
	{
		num_alive_particles = 0;
		num_requested_particles = 0;
		return;
	}

	if (vertex_buffer_)
	{
		// release current buffer
		vertex_buffer_ = nullptr;
		num_alive_particles = 0;

		// generate new vertex buffer
		D3D11_BUFFER_DESC vertex_desc;
		ZeroMemory(&vertex_desc, sizeof(vertex_desc));
		//vertex_desc.Usage = D3D11_USAGE_DEFAULT;
		num_requested_particles = ceil(lifetime_offset.y * spawn_rate_);
		int num_particles = min(MAX_RENDER_COUNT, ceil(lifetime_offset.y * spawn_rate_));
		particle_vertices_.clear();
		particles_.clear();
		particle_vertices_.reserve(num_particles);
		particles_.reserve(num_particles);

		vertex_desc.ByteWidth = sizeof(ParticleVertex) * (num_particles + 2); //  to change 
		vertex_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//vertex_desc.CPUAccessFlags = 0;
		//vertex_desc.MiscFlags = 0;
		//vertex_desc.StructureByteStride = 0;

		HRESULT result = device_->CreateBuffer(&vertex_desc, nullptr, vertex_buffer_.GetAddressOf());
#ifdef _DEBUG
		if (FAILED(result))
		{
			OutputDebugStringA("[ParticleEmitter] Failed to build a vertex buffer\n");
		}
#endif // _DEBUG
	}
}

void ParticleEmitter::SetPosOffset(XMFLOAT3 pos_offset_min, XMFLOAT3 pos_offset_max)
{
	initial_pos_offset_min_ = pos_offset_min;
	initial_pos_offset_max_ = pos_offset_max;
}

void ParticleEmitter::SetSizeOffset(XMFLOAT2 size_min, XMFLOAT2 size_max)
{
	initial_size_offset_min_ = size_min;
	initial_size_offset_max_ = size_max;

	for (int i = 0; i < num_alive_particles; i++)
	{
		particles_[i].size = { randstep(initial_size_offset_min_.x, initial_size_offset_max_.x),
				randstep(initial_size_offset_min_.y, initial_size_offset_max_.y) };
	}
}

void ParticleEmitter::SetEmitterPos(XMFLOAT3 pos)
{
	emitter_pos_ = pos;
}

void ParticleEmitter::SetVelocity(XMFLOAT3 velocity_min, XMFLOAT3 velocity_max)
{
	initial_velocity_min = velocity_min;
	initial_velocity_max = velocity_max;
}

void ParticleEmitter::SetAdditiveColor(BOOL use_random_color)
{
	use_random_color_ = use_random_color;
}
