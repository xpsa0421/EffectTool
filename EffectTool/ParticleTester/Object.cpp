#include "Object.h"

bool Object::Create(ID3D11Device* device, ID3D11DeviceContext* context, W_STR fx_path)
{
	device_ = device;
	immediate_context_ = context;

	BuildConstantBuffer();
	BuildVertexBuffer();
	BuildIndexBuffer();
	BuildFX(fx_path);
	BuildVertexLayout();

	return true;
}

void Object::BuildVertexBuffer()
{
	vertices_.resize(8);
	vertices_ =
	{
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), {1.0f, 1.0f, 1.0f, 1.0f} },
		{ DirectX::XMFLOAT3(-1.0f, +1.0f, -1.0f), {0.0f, 0.0f, 0.0f, 1.0f} },
		{ DirectX::XMFLOAT3(+1.0f, +1.0f, -1.0f), {1.0f, 0.0f, 0.0f, 1.0f} },
		{ DirectX::XMFLOAT3(+1.0f, -1.0f, -1.0f), {0.0f, 1.0f, 0.0f, 1.0f} },
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, +1.0f), {0.0f, 0.0f, 1.0f, 1.0f} },
		{ DirectX::XMFLOAT3(-1.0f, +1.0f, +1.0f), {1.0f, 1.0f, 0.0f, 1.0f} },
		{ DirectX::XMFLOAT3(+1.0f, +1.0f, +1.0f), {0.0f, 1.0f, 1.0f, 1.0f} },
		{ DirectX::XMFLOAT3(+1.0f, -1.0f, +1.0f), {1.0f, 0.0f, 1.0f, 1.0f} }
	};

	D3D11_BUFFER_DESC vertex_desc;
	vertex_desc.Usage = D3D11_USAGE_DEFAULT;
	vertex_desc.ByteWidth = sizeof(Vertex) * UINT(vertices_.size());
	vertex_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_desc.CPUAccessFlags = 0;
	vertex_desc.MiscFlags = 0;
	vertex_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertex_sub_data;
	vertex_sub_data.pSysMem = &vertices_.at(0);

	HRESULT result = device_->CreateBuffer(&vertex_desc, &vertex_sub_data, &vertex_buffer_);
#ifdef _DEBUG
	if (FAILED(result))
	{
		OutputDebugStringA("Failed to build a vertex buffer\n");
	}
#endif // _DEBUG
}

void Object::BuildIndexBuffer()
{
	indices_.resize(36);
	indices_ = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	D3D11_BUFFER_DESC index_desc;
	index_desc.Usage = D3D11_USAGE_DEFAULT;
	index_desc.ByteWidth = sizeof(UINT) * UINT(indices_.size());
	index_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	index_desc.CPUAccessFlags = 0;
	index_desc.MiscFlags = 0;
	index_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA index_sub_data;
	index_sub_data.pSysMem = &indices_.at(0);

	HRESULT result = device_->CreateBuffer(&index_desc, &index_sub_data, &index_buffer_);
#ifdef _DEBUG
	if (FAILED(result))
	{
		OutputDebugStringA("Failed to build an index buffer\n");
	}
#endif // _DEBUG
}

void Object::BuildFX(W_STR fx_path)
{
	DWORD shader_flags = 0;
#ifdef _DEBUG
	shader_flags |= D3D10_SHADER_DEBUG;
	shader_flags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	HRESULT result;
	ComPtr<ID3DBlob> shader = nullptr;
	ComPtr<ID3DBlob> err = nullptr;

	result = D3DCompileFromFile(fx_path.c_str(), 0, 0, 0, "fx_5_0", shader_flags,
		0, shader.GetAddressOf(), err.GetAddressOf());

	if (err != 0)
	{
#if defined( _DEBUG ) 
		OutputDebugStringA((char*)err->GetBufferPointer());
#endif
	}

	if (FAILED(result))
	{
#if defined( _DEBUG ) 
		OutputDebugStringA("Failed to compile FX file\n");
#endif
	}

	result = D3DX11CreateEffectFromMemory(shader->GetBufferPointer(), shader->GetBufferSize(),
		0, device_.Get(), fx_.GetAddressOf());
#ifdef _DEBUG
	if (FAILED(result))
	{
		OutputDebugStringA("Failed to build a fx interface\n");
	}
#endif // _DEBUG

	tech_ = fx_->GetTechniqueByName("Color");
}

void Object::BuildVertexLayout()
{
	D3D11_INPUT_ELEMENT_DESC vertex_desc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	D3DX11_PASS_DESC pass_desc;
	tech_->GetPassByIndex(0)->GetDesc(&pass_desc);

	HRESULT result = device_->CreateInputLayout(vertex_desc,
		sizeof(vertex_desc) / sizeof(vertex_desc[0]), pass_desc.pIAInputSignature,
		pass_desc.IAInputSignatureSize, &vertex_layout_);
#ifdef _DEBUG
	if (FAILED(result))
	{
		printf("Failed to build a vertex layout\n");
	}
#endif // _DEBUG
}

void Object::BuildConstantBuffer()
{
	D3D11_BUFFER_DESC constant_desc;
	ZeroMemory(&constant_desc, sizeof(constant_desc));
	constant_desc.ByteWidth = sizeof(ConstantData) * 1;
	constant_desc.Usage = D3D11_USAGE_DEFAULT;
	constant_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA constant_sub_data;
	ZeroMemory(&constant_sub_data, sizeof(constant_sub_data));
	constant_sub_data.pSysMem = &constant_data_;

	HRESULT result = device_->CreateBuffer(&constant_desc, &constant_sub_data, &constant_buffer_);
#ifdef _DEBUG
	if (FAILED(result))
	{
		printf("Failed to build a constant buffer\n");
	}
#endif // _DEBUG

	UpdateConstantBuffer();
}

void Object::UpdateConstantBuffer()
{
	constant_data_.world_view_proj = world_ * view_ * proj_;
	constant_data_.world_view_proj = DirectX::XMMatrixTranspose(constant_data_.world_view_proj);
	immediate_context_->UpdateSubresource(constant_buffer_.Get(), 0, 0, &constant_data_, 0, 0);
}

void Object::SetTransformationMatrix(DirectX::XMMATRIX* world, DirectX::XMMATRIX* view, DirectX::XMMATRIX* proj)
{
	if (world)	world_ = *world;
	if (view)	view_ = *view;
	if (proj)	proj_ = *proj;
	UpdateConstantBuffer();
}

bool Object::Init()
{
	device_				= nullptr;
	immediate_context_	= nullptr;
	index_buffer_		= nullptr;
	vertex_buffer_		= nullptr;
	constant_buffer_	= nullptr;
	vertex_layout_		= nullptr;
	fx_					= nullptr;
	tech_				= nullptr;

	world_ = DirectX::XMMatrixIdentity();
	view_ = DirectX::XMMatrixIdentity();
	proj_ = DirectX::XMMatrixIdentity();

	return true;
}

bool Object::Frame()
{
	return true;
}

bool Object::PreRender()
{
	UpdateConstantBuffer();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	immediate_context_->IASetInputLayout(vertex_layout_.Get());
	immediate_context_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediate_context_->IASetVertexBuffers(0, 1, vertex_buffer_.GetAddressOf(), &stride, &offset);
	immediate_context_->IASetIndexBuffer(index_buffer_.Get(), DXGI_FORMAT_R32_UINT, offset);
	immediate_context_->VSSetConstantBuffers(0, 1, &constant_buffer_);

	return true;
}

bool Object::PostRender()
{
	return true;
}

bool Object::Render()
{
	PreRender();

	D3DX11_TECHNIQUE_DESC tech_desc;
	tech_->GetDesc(&tech_desc);
	for (UINT pass_idx = 0; pass_idx < tech_desc.Passes; pass_idx++)
	{
		tech_->GetPassByIndex(pass_idx)->Apply(0, immediate_context_.Get());
		immediate_context_->DrawIndexed(UINT(indices_.size()), 0, 0);
	}

	PostRender();
	return true;
}

bool Object::Release()
{
	vertices_.clear();
	indices_.clear();

	return true;
}

