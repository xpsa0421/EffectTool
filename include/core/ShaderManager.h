#pragma once
#include "Shader.h"

class ShaderManager : public Singleton<ShaderManager>
{
private:
	friend class Singleton<ShaderManager>;
	ShaderManager() {};

private:
	VertexShader*	FindVertexShader(W_STR vs_id);
	GeometryShader*	FindGeometryShader(W_STR gs_id);
	PixelShader*	FindPixelShader(W_STR ps_id);

public:
	void			Init();
	bool			Release();
	void			SetDevice(ID3D11Device* device);

	VertexShader*	LoadVertexShader(W_STR vs_filepath, W_STR vs_func_name = L"main");
	GeometryShader*	LoadGeometryShader(W_STR gs_filepath, W_STR gs_func_name = L"main");
	PixelShader*	LoadPixelShader(W_STR ps_filepath, W_STR ps_func_name = L"main");

private:
	std::map<W_STR, VertexShader*>		vertex_shaders_;
	std::map<W_STR, GeometryShader*>	geo_shaders_;
	std::map<W_STR, PixelShader*>		pixel_shaders_;
	ComPtr<ID3D11Device> device_;
};

#define shader_manager ShaderManager::GetInstance()