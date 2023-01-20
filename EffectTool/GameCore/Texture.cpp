#include "Texture.h"

Texture::Texture()
{
	_texture	= nullptr;
	_textureSRV = nullptr;
}

Texture::~Texture()
{
	Release();
}

HRESULT	Texture::Create(ID3D11Device* device, ID3D11DeviceContext* context, W_STR texFilePath)
{
	HRESULT result = DirectX::CreateWICTextureFromFileEx(
		device,
		context,
		texFilePath.c_str(),
		0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
		DirectX::DX11::WIC_LOADER_FLAGS::WIC_LOADER_IGNORE_SRGB,
		(ID3D11Resource**)&_texture, &_textureSRV
	);
	if (FAILED(result))
	{
		result = DirectX::CreateDDSTextureFromFile(
			device,
			context,
			texFilePath.c_str(),
			(ID3D11Resource**)&_texture, &_textureSRV
		);
	}

	if (_texture) _texture->GetDesc(&_textureDesc);
	return result;
}

void Texture::Apply(ID3D11DeviceContext* context, UINT startSlot)
{
	context->PSSetShaderResources(startSlot, 1, &_textureSRV);
}

bool Texture::Release()
{
	if (_texture)		_texture->Release();
	if (_textureSRV)	_textureSRV->Release();
	return true;
}