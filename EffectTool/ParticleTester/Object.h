#pragma once
#include "Std.h"
#include <DirectXMath.h>
#include <d3dx11Effect.h>

struct Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
};

struct ConstantData
{
	DirectX::XMMATRIX world_view_proj;
};

class Object
{
public:
	Object();
	~Object();

	virtual bool	Create(ID3D11Device* device, ID3D11DeviceContext* context, W_STR fx_path);
	virtual void	BuildVertexBuffer();
	virtual void	BuildIndexBuffer();
	virtual void	BuildFX(W_STR fx_path);
	virtual void	BuildVertexLayout();
	virtual void	BuildConstantBuffer();
	virtual void	UpdateConstantBuffer();

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
	ComPtr<ID3D11InputLayout> vertex_layout_;

	ComPtr<ID3DX11Effect> fx_;
	ComPtr<ID3DX11EffectTechnique> tech_;
	ComPtr<ID3DX11EffectMatrixVariable> fx_world_view_proj_;

	DirectX::XMFLOAT4X4 world_;
	DirectX::XMFLOAT4X4 view_;
	DirectX::XMFLOAT4X4 proj_;
	ConstantData		constant_data_;

	std::vector<Vertex> vertices_;
	std::vector<UINT> indices_;
};

