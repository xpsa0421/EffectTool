#pragma once
#include "Object.h"

struct CdPerFrame
{

};

struct CdPerParticle
{

};

class Particle : public Object
{
public:
	bool	Init();
	bool	Release();

private:
	bool	PreRender();
	void	CreateVertexData();
	void	BuildConstantBuffer();
	void	BuildInputLayout();

public:
	bool	Frame();
	bool	Create(ID3D11Device* device, ID3D11DeviceContext* context);
	void	UpdateConstantBuffer();
	void	SetTransformationMatrix(XMFLOAT4X4* world, XMFLOAT4X4* view, XMFLOAT4X4* proj);

private:
	std::vector<ID3D11Buffer*>	constant_buffers_;
};

