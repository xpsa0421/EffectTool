#include "Shader.h"

//-----------------------------------------------------------------------------
// Vertex shader
//-----------------------------------------------------------------------------
VertexShader::VertexShader()
{
	device_		= nullptr;
	_shader		= nullptr;
	_VSCode		= nullptr;
}

HRESULT	VertexShader::Create(ID3D11Device* device, W_STR VSFilePath, W_STR VSFuncName)
{
	device_ = device;
	HRESULT result;
	ID3DBlob* errorCode = nullptr;
	DWORD shaderFlags = D3DCOMPILE_SKIP_OPTIMIZATION;
	
	result = D3DCompileFromFile(
		VSFilePath.c_str(),
		nullptr,
		nullptr,
		wtm(VSFuncName).c_str(),
		"vs_5_0",
		shaderFlags,
		0,
		&_VSCode,
		&errorCode
	);
	if (FAILED(result))
	{
		if (errorCode)
		{
			#if defined( _DEBUG ) 
			OutputDebugStringA((char*)errorCode->GetBufferPointer());
			#endif
			errorCode->Release();
		}
		return E_FAIL;
	}

	result = device_->CreateVertexShader(
		_VSCode->GetBufferPointer(),
		_VSCode->GetBufferSize(),
		nullptr,
		&_shader
	);

	return result;
}

bool VertexShader::Release()
{
	if (_shader)	_shader->Release();
	if (_VSCode)	_VSCode->Release();

	return true;
}


//-----------------------------------------------------------------------------
// Pixel shader
//-----------------------------------------------------------------------------

PixelShader::PixelShader()
{
	device_ = nullptr;
	_shader = nullptr;
	_PSCode = nullptr;
}

HRESULT	PixelShader::Create(ID3D11Device* device, W_STR PSFilePath, W_STR PSFuncName)
{
	device_ = device;
	HRESULT result;
	ID3DBlob* errorCode = nullptr;
	DWORD shaderFlags = D3DCOMPILE_SKIP_OPTIMIZATION;

	result = D3DCompileFromFile(
		PSFilePath.c_str(),
		nullptr,
		nullptr,
		wtm(PSFuncName).c_str(),
		"ps_5_0",
		shaderFlags,
		0,
		&_PSCode,
		&errorCode
	);

	if (FAILED(result))
	{
		if (errorCode)
		{
#if defined( _DEBUG ) 
			OutputDebugStringA((char*)errorCode->GetBufferPointer());
#endif
			errorCode->Release();
		}
		return E_FAIL;
	}

	result = device_->CreatePixelShader(
		_PSCode->GetBufferPointer(),
		_PSCode->GetBufferSize(),
		nullptr,
		&_shader
	);
	if (FAILED(result)) return E_FAIL;

	return result;
}

bool PixelShader::Release()
{
	if (_shader)	_shader->Release();
	if (_PSCode)	_PSCode->Release();

	return true;
}


