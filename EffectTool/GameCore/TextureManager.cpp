#include "TextureManager.h"

void TextureManager::Init()
{
	device_			= nullptr;
	device_context_	= nullptr;
}

void TextureManager::SetDevice(ID3D11Device* device, ID3D11DeviceContext* context)
{
	device_			= device;
	device_context_	= context;
}

Texture* TextureManager::Load(W_STR filepath)
{
	HRESULT result;
	Texture* texture = Find(filepath);

	if (texture != nullptr)
	{
		return texture;
	}
	else
	{
		texture = new Texture;
		result = texture->Create(device_.Get(), device_context_.Get(), filepath);
		if (SUCCEEDED(result))
		{
			textures_.insert(std::make_pair(filepath, texture));
			return texture;
		}
		delete texture;
	}
	return nullptr;
}

Texture* TextureManager::Find(W_STR filepath)
{
	auto iter = textures_.find(filepath);
	if (iter != textures_.end())
		return iter->second;
	else
		return nullptr;
}

bool TextureManager::Release()
{
	for (auto iter : textures_)
	{
		Texture* texture = iter.second;
		delete texture;
	}
	textures_.clear();

	return true;
}