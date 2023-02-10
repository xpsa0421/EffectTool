#pragma once
#include "Shader.h"

class ShaderManager : public Singleton<ShaderManager>
{
private:
	friend class Singleton<ShaderManager>;
	ShaderManager();

public:
	~ShaderManager();

private:
	VertexShader*	FindVertexShader(W_STR VSName);
	PixelShader*	FindPixelShader(W_STR PSName);

public:
	void			SetDevice(ID3D11Device* device);
	VertexShader*	LoadVertexShader(W_STR VSFilePath, W_STR VSFuncName);
	PixelShader*	LoadPixelShader(W_STR PSFilePath, W_STR PSFuncName);
	bool			Release();

private:
	std::map<W_STR, VertexShader*>	_vertexShaders;
	std::map<W_STR, PixelShader*>	_pixelShaders;

public:
	ComPtr<ID3D11Device> device_;
};

#define shader_manager ShaderManager::GetInstance()