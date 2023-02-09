#pragma once
#include "Std.h"

class VertexShader
{
public:
	ID3D11Device*			device_;
	ID3D11VertexShader*		_shader;
	ID3DBlob*				_VSCode;
public:
	HRESULT Create(ID3D11Device* device, W_STR VSFilePath, W_STR VSFuncName);
	bool	Release();

	VertexShader();
};

class PixelShader
{
public:
	ID3D11Device*		device_;
	ID3D11PixelShader*	_shader;
	ID3DBlob*			_PSCode;
public:
	HRESULT Create(ID3D11Device* device, W_STR PSFilePath, W_STR PSFuncName);
	bool	Release();

	PixelShader();
};