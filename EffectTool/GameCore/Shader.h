#pragma once
#include "Std.h"

class Shader
{
protected:
	Shader() {};

public:
	virtual void	Init();
	virtual bool	Release();
	virtual HRESULT	Create(ID3D11Device* device, W_STR filepath, W_STR func_name) = 0;
	
public:
	ComPtr<ID3DBlob>	code_;
};

class VertexShader : public Shader
{
public:
	void		Init()	override;
	bool		Release() override;
	HRESULT		Create(ID3D11Device* device, W_STR vs_filepath, W_STR vs_func_name) override;
	
public:
	ComPtr<ID3D11VertexShader>		shader_;
};

class GeometryShader : public Shader
{
public:
	void		Init()	override;
	bool		Release() override;
	HRESULT		Create(ID3D11Device* device, W_STR vs_filepath, W_STR vs_func_name) override;

public:
	ComPtr<ID3D11GeometryShader>	shader_;
};

class PixelShader : public Shader
{
public:
	void		Init()	override;
	bool		Release() override;
	HRESULT		Create(ID3D11Device* device, W_STR vs_filepath, W_STR vs_func_name) override;

public:
	ComPtr<ID3D11PixelShader>		shader_;
};