#pragma once
#include "Std.h"
#include <d3dx11Effect.h>

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
};

struct ConstantData
{
	XMMATRIX world_view_proj;
};

class Object
{
public:
	virtual bool	Create(ID3D11Device* device, ID3D11DeviceContext* context, W_STR fx_path);
	virtual void	BuildVertexBuffer();
	virtual void	BuildIndexBuffer();
	virtual void	BuildFX(W_STR fx_path);
	virtual void	BuildVertexLayout();
	virtual void	BuildConstantBuffer();
	virtual void	UpdateConstantBuffer();
	virtual void	SetTransformationMatrix(XMMATRIX* world, XMMATRIX* view, XMMATRIX* proj);

	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	PreRender();
	virtual bool	PostRender();
	virtual bool	Render();
	virtual bool	Release();

private:
	ComPtr<ID3D11Device> device_;
	ComPtr<ID3D11DeviceContext> immediate_context_;

	ComPtr<ID3D11Buffer> index_buffer_;
	ComPtr<ID3D11Buffer> vertex_buffer_;
	ComPtr<ID3D11Buffer> constant_buffer_;
	ComPtr<ID3D11InputLayout> vertex_layout_;

	ComPtr<ID3DX11Effect> fx_;
	ComPtr<ID3DX11EffectTechnique> tech_;

	DirectX::XMMATRIX world_;
	DirectX::XMMATRIX view_;
	DirectX::XMMATRIX proj_;
	ConstantData constant_data_;

	std::vector<Vertex> vertices_;
	std::vector<UINT> indices_;
};

