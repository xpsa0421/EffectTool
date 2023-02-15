#include "Object.h"

bool Object::Init()
{
	device_ = nullptr;
	device_context_ = nullptr;

	index_buffer_ = nullptr;
	vertex_buffer_ = nullptr;
	constant_buffer_ = nullptr;
	input_layout_ = nullptr;

	texture_ = nullptr;
	texture_srv_ = nullptr;

	vertex_shader_ = nullptr;
	geo_shader_ = nullptr;
	pixel_shader_ = nullptr;

	XMStoreFloat4x4(&world_, XMMatrixIdentity());
	XMStoreFloat4x4(&view_, XMMatrixIdentity());
	XMStoreFloat4x4(&proj_, XMMatrixIdentity());

	return true;
}

bool Object::Frame()
{
	return true;
}

bool Object::PreRender()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	device_context_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	device_context_->IASetVertexBuffers(0, 1, vertex_buffer_.GetAddressOf(), &stride, &offset);
	device_context_->IASetIndexBuffer(index_buffer_.Get(), DXGI_FORMAT_R32_UINT, 0);
	device_context_->IASetInputLayout(input_layout_.Get());
	device_context_->VSSetConstantBuffers(0, 1, constant_buffer_.GetAddressOf());
	device_context_->VSSetShader(vertex_shader_->shader_.Get(), NULL, 0);
	device_context_->PSSetShader(pixel_shader_->shader_.Get(), NULL, 0);
	device_context_->PSSetShaderResources(0, 1, texture_srv_.GetAddressOf());
	if (geo_shader_) device_context_->GSSetShader(geo_shader_->shader_.Get(), NULL, 0);

	return true;
}

bool Object::PostRender()
{
	if (index_buffer_ == nullptr)
		device_context_->Draw(UINT(vertices_.size()), 0);
	else
		device_context_->DrawIndexed(UINT(indices_.size()), 0, 0);

	return true;
}

bool Object::Render()
{
	PreRender();
	PostRender();

	return true;
}

bool Object::Release()
{
	index_buffer_ = nullptr;
	vertex_buffer_ = nullptr;
	constant_buffer_ = nullptr;
	input_layout_ = nullptr;
	
	texture_ = nullptr;
	texture_srv_ = nullptr;
	vertex_shader_ = nullptr;
	geo_shader_ = nullptr;
	pixel_shader_ = nullptr;

	vertices_.clear();
	indices_.clear();

	return true;
}

bool Object::Create(ID3D11Device* device, ID3D11DeviceContext* context)
{
	device_ = device;
	device_context_ = context;

	// generate buffers
	BuildConstantBuffer();
	BuildVertexBuffer();
	BuildIndexBuffer();

	// generate input layout
	BuildInputLayout();

	return true;
}

void Object::SetVertexShader(W_STR filepath, W_STR func_name)
{
	vertex_shader_ = shader_manager.LoadVertexShader(filepath, func_name);

	#ifdef _DEBUG
	if (vertex_shader_ == nullptr)
	{
		OutputDebugStringA(("Failed to build a vertex shader: " + wtm(filepath) + "\n").c_str());
	}
	#endif // _DEBUG
}

void Object::SetGeometryShader(W_STR filepath, W_STR func_name)
{
	geo_shader_ = shader_manager.LoadGeometryShader(filepath, func_name);

#ifdef _DEBUG
	if (geo_shader_ == nullptr)
	{
		OutputDebugStringA(("Failed to set geometry shader: " + wtm(filepath) + "\n").c_str());
	}
#endif // _DEBUG
}

void Object::SetPixelShader(W_STR filepath, W_STR func_name)
{
	pixel_shader_ = shader_manager.LoadPixelShader(filepath, func_name);
	
	#ifdef _DEBUG
	if (pixel_shader_ == nullptr)
	{
		OutputDebugStringA(("Failed to set pixel shader: " + wtm(filepath) + "\n").c_str());
	}
	#endif // _DEBUG
}

void Object::SetTexture(W_STR filepath)
{
	if (filepath == L"") return;
	texture_ = texture_manager.Load(filepath);

	#ifdef _DEBUG
	if (texture_ == nullptr)
	{
		OutputDebugStringA(("Failed to set texture: " + wtm(filepath) + "\n").c_str());
	}
	#endif // _DEBUG
	else
	{
		texture_srv_ = texture_->subresource_;
	}
}

void Object::SetTextureSRV(ID3D11ShaderResourceView* srv)
{
	texture_srv_ = srv;
}

void Object::UpdateConstantBuffer()
{
	XMMATRIX world_m = XMLoadFloat4x4(&world_);
	XMMATRIX view_m = XMLoadFloat4x4(&view_);
	XMMATRIX proj_m = XMLoadFloat4x4(&proj_);
	XMMATRIX world_view_proj_m = XMMatrixTranspose(world_m * view_m * proj_m);

	XMStoreFloat4x4(&constant_data_.world_view_proj, world_view_proj_m);
	device_context_->UpdateSubresource(constant_buffer_.Get(), 0, 0, &constant_data_, 0, 0);
}

void Object::Update(XMFLOAT4X4* world, XMFLOAT4X4* view, XMFLOAT4X4* proj)
{
	if (world)	world_ = *world;
	if (view)	view_ = *view;
	if (proj)	proj_ = *proj;

	UpdateConstantBuffer();
}

void Object::CreateVertexData()
{
	vertices_.resize(4);

	vertices_[0].pos = { -1.0f, 1.0f, 0.0f };
	vertices_[0].color = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertices_[0].tex_coord = { 0.0f, 0.0f };

	vertices_[1].pos = { 1.0f, 1.0f, 0.0f };
	vertices_[1].color = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertices_[1].tex_coord = { 1.0f, 0.0f };

	vertices_[2].pos = { -1.0f, -1.0f, 0.0f };
	vertices_[2].color = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertices_[2].tex_coord = { 0.0f, 1.0f };

	vertices_[3].pos = { 1.0f, -1.0f, 0.0f };
	vertices_[3].color = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertices_[3].tex_coord = { 1.0f, 1.0f };
}

void Object::CreateIndexData()
{
	indices_.resize(6);

	indices_[0] = 0;
	indices_[1] = 1;
	indices_[2] = 2;
	indices_[3] = 2;
	indices_[4] = 1;
	indices_[5] = 3;
}

void Object::BuildVertexBuffer()
{
	CreateVertexData();
	
	// generate vertex buffer
	D3D11_BUFFER_DESC vertex_desc;
	vertex_desc.Usage = D3D11_USAGE_DEFAULT;
	vertex_desc.ByteWidth = sizeof(Vertex) * UINT(vertices_.size());
	vertex_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_desc.CPUAccessFlags = 0;
	vertex_desc.MiscFlags = 0;
	vertex_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertex_sub_data;
	vertex_sub_data.pSysMem = &vertices_.at(0);

	HRESULT result = device_->CreateBuffer(&vertex_desc, &vertex_sub_data, vertex_buffer_.GetAddressOf());
#ifdef _DEBUG
	if (FAILED(result))
	{
		OutputDebugStringA("Failed to build a vertex buffer\n");
	}
#endif // _DEBUG
}

void Object::BuildIndexBuffer()
{
	CreateIndexData();
	
	// generate index buffer
	D3D11_BUFFER_DESC index_desc;
	index_desc.Usage = D3D11_USAGE_DEFAULT;
	index_desc.ByteWidth = sizeof(UINT) * UINT(indices_.size());
	index_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	index_desc.CPUAccessFlags = 0;
	index_desc.MiscFlags = 0;
	index_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA index_sub_data;
	index_sub_data.pSysMem = &indices_.at(0);

	HRESULT result = device_->CreateBuffer(&index_desc, &index_sub_data, index_buffer_.GetAddressOf());
#ifdef _DEBUG
	if (FAILED(result))
	{
		OutputDebugStringA("Failed to build an index buffer\n");
	}
#endif // _DEBUG
}

void Object::BuildConstantBuffer()
{
	// generate constant buffer
	D3D11_BUFFER_DESC constant_desc;
	ZeroMemory(&constant_desc, sizeof(constant_desc));
	constant_desc.ByteWidth = sizeof(ConstantData) * 1;
	constant_desc.Usage = D3D11_USAGE_DEFAULT;
	constant_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA constant_sub_data;
	ZeroMemory(&constant_sub_data, sizeof(constant_sub_data));
	constant_sub_data.pSysMem = &constant_data_;

	HRESULT result = device_->CreateBuffer(&constant_desc, &constant_sub_data, constant_buffer_.GetAddressOf());
#ifdef _DEBUG
	if (FAILED(result))
	{
		printf("Failed to build a constant buffer\n");
	}
#endif // _DEBUG

	UpdateConstantBuffer();
}

void Object::BuildInputLayout()
{
	_ASSERT(vertex_shader_);
	D3D11_INPUT_ELEMENT_DESC vertex_descs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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
		printf("Failed to build input layout\n");
	}
#endif // _DEBUG
}



