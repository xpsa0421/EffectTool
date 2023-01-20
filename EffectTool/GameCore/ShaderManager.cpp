#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	_device = nullptr;
}

ShaderManager::~ShaderManager()
{
	Release();
}

void ShaderManager::SetDevice(ID3D11Device* device)
{
	_device = device;
}

VertexShader* ShaderManager::LoadVertexShader(W_STR VSFilePath, W_STR VSFuncName)
{
	HRESULT result;
	W_STR VSName = VSFilePath + VSFuncName;
	VertexShader* vertexShader = FindVertexShader(VSName);

	if (vertexShader != nullptr)
	{
		return vertexShader;
	}
	else
	{
		vertexShader = new VertexShader;
		result = vertexShader->Create(_device, VSFilePath, VSFuncName);
		if (SUCCEEDED(result))
		{
			_vertexShaders.insert(std::make_pair(VSName, vertexShader));
		}
		return vertexShader;
	}
}

VertexShader* ShaderManager::FindVertexShader(W_STR VSName)
{
	auto VSIter = _vertexShaders.find(VSName);
	if (VSIter != _vertexShaders.end())
		return VSIter->second;
	else
		return nullptr;
}

PixelShader* ShaderManager::LoadPixelShader(W_STR PSFilePath, W_STR PSFuncName)
{
	HRESULT result;
	W_STR PSName = PSFilePath + PSFuncName;
	PixelShader* pixelShader = FindPixelShader(PSName);

	if (pixelShader != nullptr)
	{
		return pixelShader;
	}
	else
	{
		pixelShader = new PixelShader;
		result = pixelShader->Create(_device, PSFilePath, PSFuncName);
		if (SUCCEEDED(result))
		{
			_pixelShaders.insert(std::make_pair(PSName, pixelShader));
		}
		return pixelShader;
	}
}

PixelShader* ShaderManager::FindPixelShader(W_STR PSName)
{
	auto PSIter = _pixelShaders.find(PSName);
	if (PSIter != _pixelShaders.end())
		return PSIter->second;
	else
		return nullptr;
}

bool ShaderManager::Release()
{
	for (auto VSIter : _vertexShaders)
	{
		VertexShader* vertexShader = VSIter.second;
		if (vertexShader) vertexShader->Release();
		delete vertexShader;
	}
	_vertexShaders.clear();

	for (auto PSIter : _pixelShaders)
	{
		PixelShader* pixelShader = PSIter.second;
		if (pixelShader) pixelShader->Release();
		delete pixelShader;
	}
	_pixelShaders.clear();

	return true;
}
