#pragma once
#include "Std.h"

class Shader
{
private:
	Shader() = delete;
public:
	virtual void	Init();
	virtual HRESULT	Create(ID3D11Device* device, W_STR VSFilePath, W_STR VSFuncName) = 0;
	virtual bool	Release();
public:
	ComPtr<ID3DBlob>	code_;
};

class VertexShader : public Shader
{
public:
	void		Init()	override;
	HRESULT		Create(ID3D11Device* device, W_STR VSFilePath, W_STR VSFuncName) override;
	bool		Release() override;

public:
	ComPtr<ID3D11VertexShader>	shader_;
};

class PixelShader : public Shader
{
public:
	void		Init()	override;
	HRESULT		Create(ID3D11Device* device, W_STR VSFilePath, W_STR VSFuncName) override;
	bool		Release() override;

public:
	ComPtr<ID3D11PixelShader>	shader_;
};