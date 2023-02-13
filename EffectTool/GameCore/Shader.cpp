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
	return true;
}

//-----------------------------------------------------------------------------
// Vertex shader
//-----------------------------------------------------------------------------
void VertexShader::Init()
{
	Shader::Init();
	shader_	= nullptr;
}

HRESULT	VertexShader::Create(ID3D11Device* device, W_STR vs_filepath, W_STR vs_func_name)
{
	ComPtr<ID3DBlob> errorCode = nullptr;
	DWORD shaderFlags = D3DCOMPILE_SKIP_OPTIMIZATION;
	
	HRESULT result = D3DCompileFromFile(
		vs_filepath.c_str(),
		nullptr,
		nullptr,
		wtm(vs_func_name).c_str(),
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
// Geometry shader
//-----------------------------------------------------------------------------
void GeometryShader::Init()
{
	Shader::Init();
	shader_ = nullptr;
}

HRESULT	GeometryShader::Create(ID3D11Device* device, W_STR gs_filepath, W_STR gs_func_name)
{
	ComPtr<ID3DBlob> errorCode = nullptr;
	DWORD shaderFlags = D3DCOMPILE_SKIP_OPTIMIZATION;

	HRESULT result = D3DCompileFromFile(
		gs_filepath.c_str(),
		nullptr,
		nullptr,
		wtm(gs_func_name).c_str(),
		"gs_5_0",
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

	result = device->CreateGeometryShader(code_->GetBufferPointer(),
		code_->GetBufferSize(), nullptr, shader_.GetAddressOf());

	return result;
}

bool GeometryShader::Release()
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

HRESULT	PixelShader::Create(ID3D11Device* device, W_STR ps_filepath, W_STR ps_func_name)
{
	ComPtr<ID3DBlob> errorCode = nullptr;
	DWORD shaderFlags = D3DCOMPILE_SKIP_OPTIMIZATION;
	
	HRESULT result = D3DCompileFromFile(
		ps_filepath.c_str(),
		nullptr,
		nullptr,
		wtm(ps_func_name).c_str(),
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
