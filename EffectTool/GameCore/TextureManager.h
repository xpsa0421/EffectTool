#pragma once
#include "Texture.h"

class TextureManager : public Singleton<TextureManager>
{
private:
	friend class Singleton<TextureManager>;
	TextureManager() {};

public:
	void		Init();
	void		SetDevice(ID3D11Device* device, ID3D11DeviceContext* context);
	Texture*	Load(W_STR filepath);
	Texture*	Find(W_STR filepath);
	bool		Release();

private:
	ComPtr<ID3D11Device>			device_;
	ComPtr<ID3D11DeviceContext>		device_context_;
	std::map<W_STR, Texture*>		textures_;
};

#define texture_manager TextureManager::GetInstance()