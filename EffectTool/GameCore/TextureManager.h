#pragma once
#include "Texture.h"

class TextureManager : public Singleton<TextureManager>
{
private:
	friend class Singleton<TextureManager>;
	std::map<W_STR, Texture*> _textures;
public:
	ID3D11Device*			device_;
	ID3D11DeviceContext*	_context;
public:
	void		SetDevice(ID3D11Device* device, ID3D11DeviceContext* context);
	Texture*	Load(W_STR texFilePath);
	Texture*	Find(W_STR texFilePath);
	bool		Release();
private:
	TextureManager();
public:
	~TextureManager();
};

#define texture_manager TextureManager::GetInstance()