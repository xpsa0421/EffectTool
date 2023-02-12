#include "Texture.h"

void Texture::Init()
{
	texture_ = nullptr;
	subresource_ = nullptr;
	desc_ = nullptr;
}

HRESULT	Texture::Create(ID3D11Device* device, ID3D11DeviceContext* context, W_STR filepath)
{
	HRESULT result = DirectX::CreateWICTextureFromFileEx(
		device,
		context,
		filepath.c_str(),
		0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
		DirectX::DX11::WIC_LOADER_FLAGS::WIC_LOADER_IGNORE_SRGB,
		(ID3D11Resource**)texture_.GetAddressOf(), subresource_.GetAddressOf()
	);
	if (FAILED(result))
	{
		result = DirectX::CreateDDSTextureFromFile(
			device,
			context,
			filepath.c_str(),
			(ID3D11Resource**)texture_.GetAddressOf(), subresource_.GetAddressOf()
		);
	}

	if (texture_) texture_->GetDesc(desc_.Get());
	return result;
}

void Texture::Apply(ID3D11DeviceContext* context, UINT start_slot)
{
	context->PSSetShaderResources(start_slot, 1, subresource_.GetAddressOf());
}

bool Texture::Release()
{
	texture_ = nullptr;
	subresource_ = nullptr;
	desc_ = nullptr;

	return true;
}