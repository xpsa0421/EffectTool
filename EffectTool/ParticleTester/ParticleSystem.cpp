#include "ParticleSystem.h"

bool ParticleSystem::Init()
{	 
	gs_cdata_.axis_lock_type = 0;
	gs_cbuffer_ = nullptr;
	is_uv_animated_ = true; // TO CHANGE

	

	return Object::Init();
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

void ParticleSystem::CreateVertexData()
{
	particle_vertices_.resize(1);

	particle_vertices_[0].pos = { 0.0f, 0.0f, 0.0f };
	particle_vertices_[0].color = { 1.0f, 0.0f, 1.0f, 1.0f };
	particle_vertices_[0].initial_size = { 1.0f, 1.0f };
	particle_vertices_[0].tex_coord = { 0.0f, 0.0f };
	particle_vertices_[0].tex_idx = 0;
}

void ParticleSystem::BuildVertexBuffer()
{
	CreateVertexData();

	// generate vertex buffer
	D3D11_BUFFER_DESC vertex_desc;
	vertex_desc.Usage = D3D11_USAGE_DEFAULT;
	vertex_desc.ByteWidth = sizeof(ParticleVertex) * UINT(particle_vertices_.size());
	vertex_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_desc.CPUAccessFlags = 0;
	vertex_desc.MiscFlags = 0;
	vertex_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertex_sub_data;
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
	constant_sub_data.pSysMem = &gs_cdata_;

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

bool ParticleSystem::Frame()
{
	UpdateConstantBuffer();

	//EnhanceParticles();
	for (int i = 0; i < particles_.size(); i++)
	{
		particles_[i].timer += g_spf;
		particles_[i].anim_timer += g_spf;
		if (particles_[i].anim_timer >= anim_offset)
		{
			particle_vertices_[i].tex_idx++;
			if (particle_vertices_[i].tex_idx == num_textures) particle_vertices_[i].tex_idx = 0;
			particles_[i].anim_timer -= anim_offset;
		}
	}
	device_context_->UpdateSubresource(vertex_buffer_.Get(), 0, NULL,
		&particle_vertices_.at(0), 0, 0);

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
	device_context_->Draw(UINT(particle_vertices_.size()), 0);

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
	device_context_->UpdateSubresource(gs_cbuffer_.Get(), 0, 0, &gs_cdata_, 0, 0);
}

void ParticleSystem::AddTextureToArray()
{
	
}