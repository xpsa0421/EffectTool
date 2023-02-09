#pragma once
#include "Shader.h"

class ShaderManager : public Singleton<ShaderManager>
{
private:
	friend class Singleton<ShaderManager>;
	std::map<W_STR, VertexShader*>	_vertexShaders;
	std::map<W_STR, PixelShader*>	_pixelShaders;
public:
	ID3D11Device*	device_;
public:
	void			SetDevice(ID3D11Device* device);
	VertexShader*	LoadVertexShader(W_STR VSFilePath, W_STR VSFuncName);
	PixelShader*	LoadPixelShader(W_STR PSFilePath, W_STR PSFuncName);
	VertexShader*	FindVertexShader(W_STR VSName);
	PixelShader*	FindPixelShader(W_STR PSName);
	bool			Release();
private:
	ShaderManager();
public:
	~ShaderManager();
};

#define s_shaderManager ShaderManager::GetInstance()