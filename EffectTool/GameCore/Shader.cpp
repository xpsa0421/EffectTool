#include "Shader.h"

//-----------------------------------------------------------------------------
// Shader Commons
//-----------------------------------------------------------------------------
void Shader::Init()
{
	code_ = nullptr;
}

bool Shader::Release()
{
	code_ = nullptr;
}

//-----------------------------------------------------------------------------
// Vertex shader
//-----------------------------------------------------------------------------
void VertexShader::Init()
{
	Shader::Init();
	shader_	= nullptr;
}

HRESULT	VertexShader::Create(ID3D11Device* device, W_STR VSFilePath, W_STR VSFuncName)
{
	ComPtr<ID3DBlob> errorCode = nullptr;
	DWORD shaderFlags = D3DCOMPILE_SKIP_OPTIMIZATION;
	
	HRESULT result = D3DCompileFromFile(
		VSFilePath.c_str(),
		nullptr,
		nullptr,
		wtm(VSFuncName).c_str(),
		"vs_5_0",
		shaderFlags,
		0,
		code_.GetAddressOf(),
		errorCode.GetAddressOf()
	);
	if (FAILED(result))
	{
		if (errorCode)
		{
			#if defined( _DEBUG ) 
			OutputDebugStringA((char*)errorCode->GetBufferPointer());
			#endif
			errorCode = nullptr;
		}
		return E_FAIL;
	}

	result = device->CreateVertexShader(code_->GetBufferPointer(), 
						code_->GetBufferSize(),nullptr, shader_.GetAddressOf());

	return result;
}

bool VertexShader::Release()
{
	shader_ = nullptr;
	Shader::Release();

	return true;
}


//-----------------------------------------------------------------------------
// Pixel shader
//-----------------------------------------------------------------------------
void PixelShader::Init()
{
	Shader::Init();
	shader_ = nullptr;
}

HRESULT	PixelShader::Create(ID3D11Device* device, W_STR PSFilePath, W_STR PSFuncName)
{
	ComPtr<ID3DBlob> errorCode = nullptr;
	DWORD shaderFlags = D3DCOMPILE_SKIP_OPTIMIZATION;
	
	HRESULT result = D3DCompileFromFile(
		PSFilePath.c_str(),
		nullptr,
		nullptr,
		wtm(PSFuncName).c_str(),
		"ps_5_0",
		shaderFlags,
		0,
		code_.GetAddressOf(),
		errorCode.GetAddressOf()
	);
	if (FAILED(result))
	{
		if (errorCode)
		{
			#if defined( _DEBUG ) 
			OutputDebugStringA((char*)errorCode->GetBufferPointer());
			#endif
			errorCode = nullptr;
		}
		return E_FAIL;
	}

	result = device->CreatePixelShader(code_->GetBufferPointer(), 
						code_->GetBufferSize(),nullptr, shader_.GetAddressOf());

	return result;
}

bool PixelShader::Release()
{
	shader_ = nullptr;
	Shader::Release();

	return true;
}
