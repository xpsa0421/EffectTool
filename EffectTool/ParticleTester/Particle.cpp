#include "Particle.h"

bool Particle::Init()
{	 
	Object::Init();
}	 
	 
bool Particle::Release()
{

	Object::Release();
}
	 
bool Particle::PreRender()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	device_context_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	device_context_->IASetVertexBuffers(0, 1, vertex_buffer_.GetAddressOf(), &stride, &offset);
	device_context_->IASetInputLayout(input_layout_.Get());
	device_context_->VSSetConstantBuffers(0, 1, constant_buffer_.GetAddressOf());
	device_context_->VSSetShader(vertex_shader_->shader_.Get(), NULL, 0);
	device_context_->GSSetShader(geo_shader_->shader_.Get(), NULL, 0);
	device_context_->PSSetShader(pixel_shader_->shader_.Get(), NULL, 0);
	device_context_->PSSetShaderResources(0, 1, texture_srv_.GetAddressOf());

	return true;
}

void Particle::CreateVertexData()
{
	vertices_[0].pos = { -1.0f, 1.0f, 0.0f };
	vertices_[0].color = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertices_[0].tex_coord = { 0.0f, 0.0f };
}

void Particle::BuildConstantBuffer()
{

}

void Particle::BuildInputLayout()
{

}

	 
bool Particle::Frame()
{

}

bool Particle::Create(ID3D11Device* device, ID3D11DeviceContext* context)
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
	
void Particle::UpdateConstantBuffer()
{

}

void Particle::SetTransformationMatrix(XMFLOAT4X4* world, XMFLOAT4X4* view, XMFLOAT4X4* proj)
{

}

