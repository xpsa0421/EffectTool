#include "BaseObject.h"

/*----------------------------------------------------------
|	OBJECT CREATION                                         |
 -----------------------------------------------------------*/

void BaseObject::CreateConstantData()
{
	_constantData.worldMat = Matrix::Transpose(_constantData.worldMat);
	_constantData.viewMat = Matrix::Transpose(_constantData.viewMat);
	_constantData.projMat = Matrix::Transpose(_constantData.projMat);
	_constantData.colour = { 1,1,1,1 };
	_constantData.light = { 0,0,0,0 };
}

void BaseObject::CreateVertexData()
{
	if (_vertices.size() == 0)
	{
		_vertices.resize(4);

		_vertices[0].p = { -1.0f, 1.0f, 0.0f };
		_vertices[0].n = { 0.0f, 0.0f, -1.0f };
		_vertices[0].c = { 1.0f, 1.0f, 1.0f, 1.0f };
		_vertices[0].t = { 0.0f, 0.0f };

		_vertices[1].p = { 1.0f, 1.0f, 0.0f };
		_vertices[1].n = { 0.0f, 0.0f, -1.0f };
		_vertices[1].c = { 1.0f, 1.0f, 1.0f, 1.0f };
		_vertices[1].t = { 1.0f, 0.0f };

		_vertices[2].p = { -1.0f, -1.0f, 0.0f };
		_vertices[2].n = { 0.0f, 0.0f, -1.0f };
		_vertices[2].c = { 1.0f, 1.0f, 1.0f, 1.0f };
		_vertices[2].t = { 0.0f, 1.0f };

		_vertices[3].p = { 1.0f, -1.0f, 0.0f };
		_vertices[3].n = { 0.0f, 0.0f, -1.0f };
		_vertices[3].c = { 1.0f, 1.0f, 1.0f, 1.0f };
		_vertices[3].t = { 1.0f, 1.0f };
	}

	_initVertices = _vertices;
}

void BaseObject::CreateIndexData()
{
	if (_indices.size() == 0)
	{
		_indices.resize(6);

		_indices[0] = 0;
		_indices[1] = 1;
		_indices[2] = 2;
		_indices[3] = 2;
		_indices[4] = 1;
		_indices[5] = 3;
	}
}

HRESULT	BaseObject::CreateConstantBuffer()
{
	CreateConstantData();

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = sizeof(VSConstantData) * 1;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA subRsrcData;
	ZeroMemory(&subRsrcData, sizeof(subRsrcData));
	subRsrcData.pSysMem = &_constantData;

	HRESULT result = device_->CreateBuffer(
		&bufferDesc,
		&subRsrcData,
		&_constantBuffer
	);
	return result;
}

HRESULT	BaseObject::CreateVertexBuffer()
{
	CreateVertexData();

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = sizeof(Vertex) * UINT(_vertices.size());
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subRsrcData;
	ZeroMemory(&subRsrcData, sizeof(subRsrcData));
	subRsrcData.pSysMem = &_vertices.at(0);

	HRESULT result = device_->CreateBuffer(
		&bufferDesc,
		&subRsrcData,
		&_vertexBuffer
	);
	return result;
}

HRESULT	BaseObject::CreateIndexBuffer()
{
	CreateIndexData();

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = sizeof(DWORD) * UINT(_indices.size());
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subRsrcData;
	ZeroMemory(&subRsrcData, sizeof(subRsrcData));
	subRsrcData.pSysMem = &_indices.at(0);

	HRESULT result = device_->CreateBuffer(
		&bufferDesc,
		&subRsrcData,
		&_indexBuffer
	);
	return result;
}

HRESULT	BaseObject::CreateVertexLayout()
{
	D3D11_INPUT_ELEMENT_DESC elementDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	HRESULT result = device_->CreateInputLayout(
		elementDesc,
		sizeof(elementDesc) / sizeof(elementDesc[0]),
		_vertexShader->code_->GetBufferPointer(),
		_vertexShader->code_->GetBufferSize(),
		&_vertexLayout
	);
	return result;
}

bool BaseObject::LoadShader(W_STR VSFilePath, W_STR PSFilePath, W_STR VSFuncName, W_STR PSFuncName)
{
	_vertexShader = shader_manager.LoadVertexShader(VSFilePath, VSFuncName);
	_pixelShader = shader_manager.LoadPixelShader(PSFilePath, PSFuncName);
	if (_vertexShader && _pixelShader) return true;
	else return false;
}

bool BaseObject::LoadTexture(W_STR texFilePath)
{
	if (texFilePath == L"") return false;
	_texture = texture_manager.Load(texFilePath);
	if (_texture)
	{
		_textureSRV = _texture->_textureSRV;
		return true;
	}
	else return false;
}

// Creates an object without a texture, using default shaders
// TODO default function  이름 바꾸기~!!!
bool BaseObject::Create(
	ID3D11Device* device, ID3D11DeviceContext* immediateContext,
	W_STR VSFilePath, W_STR PSFilePath)
{
	device_ = device;
	device_context_ = immediateContext;

	if (FAILED(CreateConstantBuffer()))			return false;
	if (FAILED(CreateVertexBuffer()))			return false;
	if (FAILED(CreateIndexBuffer()))			return false;
	if (!LoadShader(VSFilePath, PSFilePath))	return false;
	if (FAILED(CreateVertexLayout()))			return false;
	
	Init();
	return true;
}

// Creates an object with a texture, using custom shaders
bool BaseObject::Create(
	ID3D11Device* device, ID3D11DeviceContext* immediateContext,
	W_STR VSFilePath, W_STR PSFilePath,
	W_STR VSFuncName, W_STR PSFuncName, W_STR texFilePath)
{
	device_ = device;
	device_context_ = immediateContext;

	if (FAILED(CreateConstantBuffer()))									return false;
	if (FAILED(CreateVertexBuffer()))									return false;
	if (FAILED(CreateIndexBuffer()))									return false;
	if (!LoadShader(VSFilePath, PSFilePath, VSFuncName, PSFuncName))	return false;
	if (FAILED(CreateVertexLayout()))									return false;
	if (LoadTexture(texFilePath))
	{
		_textureSRV = _texture->_textureSRV;
	};

	Init();
	return true;
}

void BaseObject::UpdateConstantBuffer()
{
	_constantData.worldMat	= Matrix::Transpose(_worldMat);
	_constantData.viewMat	= Matrix::Transpose(_viewMat); 
	_constantData.projMat	= Matrix::Transpose(_projMat); 
	device_context_->UpdateSubresource(
		_constantBuffer, NULL, nullptr, &_constantData, 0, 0);
}

void BaseObject::UpdateVertexBuffer()
{
	device_context_->UpdateSubresource(
		_vertexBuffer, NULL, nullptr, &_vertices.at(0), 0, 0);
}

void BaseObject::SetTransformationMatrix(Matrix* worldMat, Matrix* viewMat, Matrix* projMat)
{
	if (worldMat)	_worldMat = *worldMat;
	if (viewMat)	_viewMat = *viewMat;
	if (projMat)	_projMat = *projMat;
	UpdateConstantBuffer();
}

/*
Static function that creates and returns a vertex buffer
*/
ID3D11Buffer* BaseObject::CreateVertexBuffer(ID3D11Device* device, void* dataAddress, UINT numVertices, UINT vertexSize)
{
	ID3D11Buffer* vertexBuffer;
	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(vbDesc));
	vbDesc.ByteWidth = vertexSize * numVertices;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subRsrcData;
	ZeroMemory(&subRsrcData, sizeof(subRsrcData));
	subRsrcData.pSysMem = dataAddress;

	HRESULT result = device->CreateBuffer(
		&vbDesc,
		&subRsrcData,
		&vertexBuffer
	);

	return vertexBuffer;
}

/*----------------------------------------------------------
|	OBJECT RENDERING		                                |
 -----------------------------------------------------------*/

bool BaseObject::Init()
{
	return true;
}

bool BaseObject::Frame()
{
	return true;
}

bool BaseObject::PreRender()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	device_context_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	device_context_->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
	device_context_->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, offset);
	device_context_->IASetInputLayout(_vertexLayout);
	device_context_->VSSetConstantBuffers(0, 1, &_constantBuffer);
	device_context_->VSSetShader(_vertexShader->shader_, NULL, 0);
	device_context_->PSSetShader(_pixelShader->shader_, NULL, 0);
	device_context_->PSSetShaderResources(0, 1, &_textureSRV);

	return true;
}

bool BaseObject::PostRender()
{
	if (_indexBuffer == nullptr)
		device_context_->Draw(UINT(_vertices.size()), 0);
	else
		device_context_->DrawIndexed(UINT(_indices.size()), 0, 0);

	return true;
}

bool BaseObject::Render()
{
	PreRender();
	PostRender();
	return true;
}

bool BaseObject::Release()
{
	if (_constantBuffer)	_constantBuffer->Release();
	if (_indexBuffer)		_indexBuffer->Release();
	if (_vertexBuffer)		_vertexBuffer->Release();
	if (_vertexLayout)		_vertexLayout->Release();

	return true;
}
