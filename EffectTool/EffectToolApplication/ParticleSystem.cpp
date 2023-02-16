#include "pch.h"
#include "ParticleSystem.h"

bool ParticleSystem::Init()
{	 
	cd_per_system_.axis_lock_type = 0;
	gs_cbuffer_ = nullptr;
	is_uv_animated_ = true; // TO CHANGE
	initial_pos_offset_min_ = { -2,-1,-2 };
	initial_pos_offset_max_ = { 2,4,2 };
	initial_size_offset_min_ = {0.5f, 0.5f};
	initial_size_offset_max_ = { 3,3 };
	Object::Init();

	SetVertexShader(L"VertexShader.hlsl", L"main");
	SetPixelShader(L"PixelShader.hlsl", L"main");
	SetGeometryShader(L"GeometryShader.hlsl", L"main");
	return true;
}	 
	 
bool ParticleSystem::Release()
{
	gs_cbuffer_ = nullptr;
	if (!texture_)
	{
		texture_srv_->Release();
	}
	return Object::Release();
}
	 
bool ParticleSystem::PreRender()
{
	UINT stride = sizeof(ParticleVertex);
	UINT offset = 0;

	device_context_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	device_context_->IASetVertexBuffers(0, 1, vertex_buffer_.GetAddressOf(), &stride, &offset);
	device_context_->IASetInputLayout(input_layout_.Get());
	device_context_->VSSetShader(vertex_shader_->shader_.Get(), NULL, 0);
	device_context_->GSSetShader(geo_shader_->shader_.Get(), NULL, 0);
	device_context_->GSSetConstantBuffers(1, 1, gs_cbuffer_.GetAddressOf());
	device_context_->PSSetShader(pixel_shader_->shader_.Get(), NULL, 0);
	device_context_->PSSetShaderResources(0, 1, texture_srv_.GetAddressOf());
	DxState.ApplyBlendState(L"BS_dual_source");

	return true;
}

void ParticleSystem::BuildVertexBuffer()
{
	//particle_vertices_.resize(1);


	// generate vertex buffer
	D3D11_BUFFER_DESC vertex_desc;
	ZeroMemory(&vertex_desc, sizeof(vertex_desc));
	//vertex_desc.Usage = D3D11_USAGE_DEFAULT;
	vertex_desc.ByteWidth = sizeof(ParticleVertex) * 50; //  to change 
	vertex_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//vertex_desc.CPUAccessFlags = 0;
	//vertex_desc.MiscFlags = 0;
	//vertex_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertex_sub_data;
	ZeroMemory(&vertex_sub_data, sizeof(vertex_sub_data));
	vertex_sub_data.pSysMem = &particle_vertices_.at(0);

	HRESULT result = device_->CreateBuffer(&vertex_desc, &vertex_sub_data, vertex_buffer_.GetAddressOf());
	#ifdef _DEBUG
	if (FAILED(result))
	{
		OutputDebugStringA("[ParticleSystem] Failed to build a vertex buffer\n");
	}
	#endif // _DEBUG
}

void ParticleSystem::BuildConstantBuffer()
{
	// generate per particle system constant buffer
	D3D11_BUFFER_DESC constant_desc;
	ZeroMemory(&constant_desc, sizeof(constant_desc));
	constant_desc.ByteWidth = sizeof(CdPerParticleSystem) * 1;
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

void ParticleSystem::BuildInputLayout()
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
		printf("[ParticleSystem] Failed to build input layout\n");
	}
#endif // _DEBUG
}

void ParticleSystem::EnhanceParticles()
{
	for (int i = 0; i < particles_.size(); i++)
	{
		if (particles_[i].active == false) continue;
		
		if ((particles_[i].lifetime != -1) && (particles_[i].timer > particles_[i].lifetime))
		{
			particle_vertices_[i].color = { 0,0,0,0 };
			particles_[i].active = false;
			continue;
		}

		particles_[i].timer += g_spf;
		particles_[i].anim_timer += g_spf;

		// update texture
		if (particles_[i].anim_timer > anim_offset)
		{
			particle_vertices_[i].tex_idx++;
			if (particle_vertices_[i].tex_idx == num_textures) particle_vertices_[i].tex_idx = 0;
			particles_[i].anim_timer -= anim_offset;
		}
		particle_vertices_[i].pos = particles_[i].position;
		particle_vertices_[i].color = particles_[i].color;
		particle_vertices_[i].size = particles_[i].size;
	};

	device_context_->UpdateSubresource(vertex_buffer_.Get(), 0, 0,
		&particle_vertices_.at(0), 0, 0);
}

bool ParticleSystem::Frame()
{
	if (emit_range != -1)
	{
		emit_timer += g_spf;
		if (particle_vertices_.size() < 40)
		{
			if (emit_timer > emit_range)
			{
				EmitParticles();
				emit_timer -= emit_range;
			}
		}
	}
	UpdateConstantBuffer();
	EnhanceParticles();

	
	//TODO: tex coordinate 
	

	/*for (int i = 0; i < particles_.size(); i++)
	{
		particles_[i].timer += g_spf;
		if (particles_[i].timer > particles_[i].lifetime)
		{
			particles_.
		}
		m_Paticles[i].vPos = m_Paticles[i].vPos + m_Paticles[i].vVel * g_fSecondPerFrame;
		m_VertexList[i].p = m_Paticles[i].vPos;

		m_VertexList[i].n = m_Paticles[i].vVel;
		m_VertexList[i].c = m_Paticles[i].vColor;
		m_VertexList[i].t = { 0.0f, 0.0f };
	}*/
	return true;
}

bool ParticleSystem::PostRender()
{
	device_context_->Draw(particle_vertices_.size(), 0);

	return true;
}

bool ParticleSystem::Create(ID3D11Device* device, ID3D11DeviceContext* context)
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
	
void ParticleSystem::UpdateConstantBuffer()
{
	device_context_->UpdateSubresource(gs_cbuffer_.Get(), 0, 0, &cd_per_system_, 0, 0);
}

void ParticleSystem::SetUVAnimation(W_STR sprite_sheet_path, float num_rows, float num_cols)
{
	cd_per_system_.is_uv_animated = true;
	num_textures = num_rows * num_cols;
	SetTexture(sprite_sheet_path);
	cd_per_system_.sprite_dimension = { int(texture_->desc_.Width) / num_cols, int(texture_->desc_.Height) / num_rows };
}

void ParticleSystem::SetMultiTexAnimation(std::vector<W_STR>& filenames)
{
	is_uv_animated_ = false;
	num_textures = filenames.size();
	// create shader resource view from texture 2d array
	SetTextureSRV(CreateTexture2DArraySRV(device_.Get(), device_context_.Get(), filenames));
}

void ParticleSystem::SetAlphaTesting(bool is_alpha_tested)
{
	cd_per_system_.is_alpha_tested = is_alpha_tested;
}

void ParticleSystem::SetAlphaBlending(bool is_alpha_blended)
{
	cd_per_system_.is_alpha_tested = is_alpha_blended;
}

void ParticleSystem::SetLifetimeOffset(float min_lifetime, float max_lifetime)
{
	lifetime_offset = { min_lifetime, max_lifetime };
}

void ParticleSystem::EmitParticles()
{
	for (int i = 0; i < emit_num_particles; i++)
	{
		Particle p;
		p.position = {	emitter_pos_.x + randstep(initial_pos_offset_min_.x, initial_pos_offset_max_.x),
						emitter_pos_.y + randstep(initial_pos_offset_min_.y, initial_pos_offset_max_.y),
						emitter_pos_.z + randstep(initial_pos_offset_min_.z, initial_pos_offset_max_.z) };
		p.color = { randstep(0,1),randstep(0,1),randstep(0,1),1 };
		p.size = {	randstep(initial_size_offset_min_.x, initial_size_offset_max_.x),
					randstep(initial_size_offset_min_.y, initial_size_offset_max_.y) };
		p.lifetime = { randstep(lifetime_offset.x, lifetime_offset.y) };
		particles_.push_back(p);

		ParticleVertex p_v;
		particle_vertices_.push_back(p_v);
	}
}

void ParticleSystem::SetEmitterProperties(float emit_range, int emit_number)
{
	this->emit_range = emit_range;
	this->emit_num_particles = emit_number;
}

void ParticleSystem::SetPosOffset(XMFLOAT3 pos_offset_min, XMFLOAT3 pos_offset_max)
{
	initial_pos_offset_min_ = pos_offset_min;
	initial_pos_offset_max_ = pos_offset_max;
}

void ParticleSystem::SetSizeOffset(XMFLOAT2 size_min, XMFLOAT2 size_max)
{
	initial_size_offset_min_ = size_min;
	initial_size_offset_max_ = size_max;
}

void ParticleSystem::SetEmitterPos(XMFLOAT3 pos)
{
	emitter_pos_ = pos;
}