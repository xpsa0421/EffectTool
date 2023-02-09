#include "TextureManager.h"

TextureManager::TextureManager()
{
	device_		= nullptr;
	_context	= nullptr;
}

TextureManager::~TextureManager()
{
	Release();
}

void TextureManager::SetDevice(ID3D11Device* device, ID3D11DeviceContext* context)
{
	device_		= device;
	_context	= context;
}

Texture* TextureManager::Load(W_STR texFilePath)
{
	HRESULT result;
	Texture* texture = Find(texFilePath);

	if (texture != nullptr)
	{
		return texture;
	}
	else
	{
		texture = new Texture;
		result = texture->Create(device_, _context, texFilePath);
		if (SUCCEEDED(result))
		{
			_textures.insert(std::make_pair(texFilePath, texture));
			return texture;
		}
		delete texture;
	}
	return nullptr;
}

Texture* TextureManager::Find(W_STR texFilePath)
{
	auto textureIter = _textures.find(texFilePath);
	if (textureIter != _textures.end())
		return textureIter->second;
	else
		return nullptr;
}

bool TextureManager::Release()
{
	for (auto textureIter : _textures)
	{
		Texture* texture = textureIter.second;
		delete texture;
	}
	_textures.clear();
	return true;
}