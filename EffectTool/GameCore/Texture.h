#pragma once
#include "Std.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
#pragma comment(lib, "DirectXTK.lib")

class Texture
{
public:
	ID3D11Texture2D*			_texture;
	ID3D11ShaderResourceView*	_textureSRV;
	D3D11_TEXTURE2D_DESC		_textureDesc;
public:
	HRESULT	Create(ID3D11Device* device, ID3D11DeviceContext* context, W_STR texFilePath);
	void	Apply(ID3D11DeviceContext* context, UINT startSlot = 0);
	bool	Release();

	Texture();
	~Texture();
};