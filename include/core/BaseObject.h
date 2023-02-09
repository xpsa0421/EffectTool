#pragma once
#include "TextureManager.h"
#include "ShaderManager.h"
#include "CoreMath.h"

struct Vertex
{
	Vector	p;
	Vector	n;
	Vector4 c;
	Vector2 t;
	Vertex() {};
	Vertex(Vector point, Vector normal, Vector4 colour, Vector2 textureUV)
	{
		p = point;
		n = normal;
		c = colour;
		t = textureUV;
	}
};

struct VSConstantData
{
	Matrix	worldMat;
	Matrix	viewMat;
	Matrix	projMat;
	Vector4 colour;
	Vector4 light;
};

class BaseObject
{
public:
	ID3D11Device*				device_				= nullptr;
	ID3D11DeviceContext*		immediate_context_	= nullptr;
public:
	ID3D11Buffer*				_constantBuffer		= nullptr;
	ID3D11Buffer*				_indexBuffer		= nullptr;
	ID3D11Buffer*				_vertexBuffer		= nullptr;
	ID3D11InputLayout*			_vertexLayout		= nullptr;
	Texture*					_texture			= nullptr;
	ID3D11ShaderResourceView*	_textureSRV			= nullptr;
	VertexShader*				_vertexShader		= nullptr;
	PixelShader*				_pixelShader		= nullptr;
public:
	VSConstantData				_constantData;
	std::vector<Vertex>			_initVertices;
	std::vector<Vertex>			_vertices;
	std::vector<DWORD>			_indices;
public:
	Matrix	_worldMat;
	Matrix	_viewMat;
	Matrix	_projMat;
public:
	DWORD	_numFaces	= 0;
public:;
	virtual void			CreateConstantData();
	virtual void			CreateVertexData();
	virtual void			CreateIndexData();
	virtual HRESULT			CreateConstantBuffer();
	virtual HRESULT			CreateVertexBuffer();
	virtual HRESULT			CreateIndexBuffer();
	virtual HRESULT			CreateVertexLayout();
	static ID3D11Buffer*	CreateVertexBuffer(ID3D11Device* device, void* dataAddress, UINT numVertices, UINT vertexSize);
public:
	virtual bool	LoadShader(W_STR VSFilePath, W_STR PSFilePath,
		W_STR VSFuncName = L"VSMain", W_STR PSFuncName = L"PSMain");
	virtual bool	LoadTexture(W_STR texFilePath);
public:
	virtual bool	Create(ID3D11Device* device, ID3D11DeviceContext* immediateContext,
		W_STR VSFilePath, W_STR PSFilePath,
		W_STR VSFuncName, W_STR PSFuncName, W_STR texFilePath);
	virtual bool	Create(ID3D11Device* device, ID3D11DeviceContext* immediateContext,
		W_STR VSFilePath, W_STR PSFilePath);
public:
	virtual void	UpdateConstantBuffer();
	virtual void	UpdateVertexBuffer();
	virtual void	SetTransformationMatrix(Matrix* worldMat, Matrix* viewMat, Matrix* projMat);
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	PreRender();
	virtual bool	PostRender();
	virtual bool	Render();
	virtual bool	Release();
};