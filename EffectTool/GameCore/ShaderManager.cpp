#include "ShaderManager.h"

void ShaderManager::Init()
{
	device_ = nullptr;
}

bool ShaderManager::Release()
{
	for (auto vs_iter : vertex_shaders_)
	{
		VertexShader* vertex_shader = vs_iter.second;
		if (vertex_shader) vertex_shader->Release();
		delete vertex_shader;
	}
	vertex_shaders_.clear();

	for (auto gs_iter : geo_shaders_)
	{
		GeometryShader* geo_shader = gs_iter.second;
		if (geo_shader) geo_shader->Release();
		delete geo_shader;
	}
	geo_shaders_.clear();

	for (auto ps_iter : pixel_shaders_)
	{
		PixelShader* pixel_shader = ps_iter.second;
		if (pixel_shader) pixel_shader->Release();
		delete pixel_shader;
	}
	pixel_shaders_.clear();

	return true;
}

void ShaderManager::SetDevice(ID3D11Device* device)
{
	device_ = device;
}

VertexShader* ShaderManager::FindVertexShader(W_STR vs_id)
{
	auto iter = vertex_shaders_.find(vs_id);
	if (iter != vertex_shaders_.end())
		return iter->second;
	else
		return nullptr;
}

GeometryShader* ShaderManager::FindGeometryShader(W_STR gs_id)
{
	auto iter = geo_shaders_.find(gs_id);
	if (iter != geo_shaders_.end())
		return iter->second;
	else
		return nullptr;
}

PixelShader* ShaderManager::FindPixelShader(W_STR ps_id)
{
	auto iter = pixel_shaders_.find(ps_id);
	if (iter != pixel_shaders_.end())
		return iter->second;
	else
		return nullptr;
}

VertexShader* ShaderManager::LoadVertexShader(W_STR vs_filepath, W_STR vs_func_name)
{
	HRESULT result;
	W_STR vs_id = vs_filepath + vs_func_name;
	VertexShader* vertex_shader = FindVertexShader(vs_id);

	if (vertex_shader != nullptr)
	{
		return vertex_shader;
	}
	else
	{
		vertex_shader = new VertexShader;
		result = vertex_shader->Create(device_.Get(), vs_filepath, vs_func_name);
		if (SUCCEEDED(result))
		{
			vertex_shaders_.insert(std::make_pair(vs_id, vertex_shader));
		}
		return vertex_shader;
	}
}

GeometryShader* ShaderManager::LoadGeometryShader(W_STR gs_filepath, W_STR gs_func_name)
{
	HRESULT result;
	W_STR gs_id = gs_filepath + gs_func_name;
	GeometryShader* geo_shader = FindGeometryShader(gs_id);

	if (geo_shader != nullptr)
	{
		return geo_shader;
	}
	else
	{
		geo_shader = new GeometryShader;
		result = geo_shader->Create(device_.Get(), gs_filepath, gs_func_name);
		if (SUCCEEDED(result))
		{
			geo_shaders_.insert(std::make_pair(gs_id, geo_shader));
		}
		return geo_shader;
	}
}

PixelShader* ShaderManager::LoadPixelShader(W_STR ps_filepath, W_STR ps_func_name)
{
	HRESULT result;
	W_STR ps_id = ps_filepath + ps_func_name;
	PixelShader* pixel_shader = FindPixelShader(ps_id);

	if (pixel_shader != nullptr)
	{
		return pixel_shader;
	}
	else
	{
		pixel_shader = new PixelShader;
		result = pixel_shader->Create(device_.Get(), ps_filepath, ps_func_name);
		if (SUCCEEDED(result))
		{
			pixel_shaders_.insert(std::make_pair(ps_id, pixel_shader));
		}
		return pixel_shader;
	}
}
