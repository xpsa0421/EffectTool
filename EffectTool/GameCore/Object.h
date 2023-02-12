#pragma once
#include "Std.h"
#include "ShaderManager.h"
#include "TextureManager.h"

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
	XMFLOAT2 tex_coord;

	Vertex(	XMFLOAT3 p = XMFLOAT3(0, 0, 0),
			XMFLOAT4 c = XMFLOAT4(0, 0, 0, 0),
			XMFLOAT2 t = XMFLOAT2(0, 0))
	{
		pos = p;
		color = c;
		tex_coord = t;
	}
};

struct ConstantData
{
	XMFLOAT4X4 world_view_proj;
};

class Object
{
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	PreRender();
	virtual bool	PostRender();
	virtual bool	Render();
	virtual bool	Release();

	virtual bool	Create(ID3D11Device* device, ID3D11DeviceContext* context);
	virtual void	SetVertexShader(W_STR filepath, W_STR func_name);
	virtual void	SetPixelShader(W_STR filepath, W_STR func_name);
	virtual void	SetTexture(W_STR filepath);

	virtual void	UpdateConstantBuffer();
	virtual void	SetTransformationMatrix(XMFLOAT4X4* world, XMFLOAT4X4* view, XMFLOAT4X4* proj);

protected:
	virtual void	BuildVertexBuffer();
	virtual void	BuildIndexBuffer();
	virtual void	BuildConstantBuffer();
	virtual void	BuildInputLayout();

protected:
	ComPtr<ID3D11Device>		device_;
	ComPtr<ID3D11DeviceContext> device_context_;

	ComPtr<ID3D11Buffer>		index_buffer_;
	ComPtr<ID3D11Buffer>		vertex_buffer_;
	ComPtr<ID3D11Buffer>		constant_buffer_;
	ComPtr<ID3D11InputLayout>	input_layout_;

	Texture*			texture_;
	VertexShader*		vertex_shader_;
	PixelShader*		pixel_shader_;

	XMFLOAT4X4			world_;
	XMFLOAT4X4			view_;
	XMFLOAT4X4			proj_;
	ConstantData		constant_data_;

	std::vector<Vertex> vertices_;
	std::vector<UINT>	indices_;
};

