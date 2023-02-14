#include "ParticleSystem.h"

bool ParticleSystem::Init()
{	 
	return Object::Init();
}	 
	 
bool ParticleSystem::Release()
{
	gs_cbuffer_per_system = nullptr;
	return Object::Release();
}
	 
bool ParticleSystem::PreRender()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	device_context_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	device_context_->IASetVertexBuffers(0, 1, vertex_buffer_.GetAddressOf(), &stride, &offset);
	device_context_->IASetInputLayout(input_layout_.Get());
	device_context_->VSSetShader(vertex_shader_->shader_.Get(), NULL, 0);
	device_context_->GSSetShader(geo_shader_->shader_.Get(), NULL, 0);
	device_context_->GSSetConstantBuffers(1, 1, gs_cbuffer_per_system.GetAddressOf());
	device_context_->PSSetShader(pixel_shader_->shader_.Get(), NULL, 0);
	device_context_->PSSetShaderResources(0, 1, texture_srv_.GetAddressOf());

	return true;
}

void ParticleSystem::CreateVertexData()
{
	vertices_.resize(1);

	vertices_[0].pos = { 0.0f, 0.0f, 0.0f };
	vertices_[0].color = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertices_[0].tex_coord = { 0.0f, 0.0f };
}

void ParticleSystem::BuildConstantBuffer()
{
	// generate per particle system constant buffer
	D3D11_BUFFER_DESC constant_desc;
	ZeroMemory(&constant_desc, sizeof(constant_desc));
	constant_desc.ByteWidth = sizeof(CdPerParticleSystem) * 1;
	constant_desc.Usage = D3D11_USAGE_DEFAULT;
	constant_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	HRESULT result = device_->CreateBuffer(&constant_desc, nullptr, gs_cbuffer_per_system.GetAddressOf());
	#ifdef _DEBUG
	if (FAILED(result))
	{
		printf("[Particle System] Failed to build per system constant buffer\n");
	}
	#endif // _DEBUG

	UpdateConstantBuffer();
}
	 
bool ParticleSystem::Frame()
{
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
	XMMATRIX view_m = XMLoadFloat4x4(&view_);
	XMMATRIX proj_m = XMLoadFloat4x4(&proj_);
	XMMATRIX view_proj_m = XMMatrixTranspose(view_m * proj_m);

	CdPerParticleSystem gs_cdata_per_system;
	XMStoreFloat4x4(&gs_cdata_per_system.view_proj, view_proj_m);
	device_context_->UpdateSubresource(gs_cbuffer_per_system.Get(), 0, 0, &gs_cdata_per_system, 0, 0);
}