#pragma once
#include "Std.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
#pragma comment(lib, "DirectXTK.lib")

class Texture
{
public:
	void		Init();
	HRESULT		Create(ID3D11Device* device, ID3D11DeviceContext* context, W_STR texFilePath);
	void		Apply(ID3D11DeviceContext* context, UINT startSlot = 0);
	bool		Release();

public:
	ComPtr<ID3D11Texture2D>				texture_;
	ComPtr<ID3D11ShaderResourceView>	subresource_;
	D3D11_TEXTURE2D_DESC				desc_;
};