#pragma once

//-----------------------------------------------------------------------------
// Standard includes
//-----------------------------------------------------------------------------
#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgidebug.h>
#include <codecvt>
#include <tchar.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <dxtk\WICTextureLoader.h>


//-----------------------------------------------------------------------------
// Standard libraries
//-----------------------------------------------------------------------------
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
extern HWND		g_hWnd;
extern RECT		g_rectClient;
extern float	g_timer;
extern float	g_delta_time;
extern UINT		g_fps;


//-----------------------------------------------------------------------------
// Custom definitions
//-----------------------------------------------------------------------------
#define randstep(fMin,fMax) (fMin+((float)fMax-(float)fMin)*rand()/(float)RAND_MAX)


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef std::basic_string<TCHAR>				T_STR;
typedef std::basic_string<wchar_t>				W_STR;
typedef std::basic_string<char>					C_STR;
typedef std::string								STR;
typedef std::vector<std::basic_string<TCHAR>>	TSTR_VECTOR;
typedef std::vector<DWORD>						DWORD_VECTOR;
typedef std::vector<W_STR>						WSTR_VECTOR;
typedef std::vector<float>						FLOAT_VECTOR;


//-----------------------------------------------------------------------------
// Namespaces
//-----------------------------------------------------------------------------
using namespace Microsoft::WRL;
using namespace DirectX;


//-----------------------------------------------------------------------------
// Standard classes
//-----------------------------------------------------------------------------
template<class T> class Singleton
{
public:
	static T& GetInstance()
	{
		static T theSingleInstance;
		return theSingleInstance;
	}
};


//-----------------------------------------------------------------------------
// Utility functions
//-----------------------------------------------------------------------------
static W_STR mtw(STR str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.from_bytes(str);
}

static STR wtm(W_STR str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.to_bytes(str);
}

static ID3D11ShaderResourceView* CreateSRVFromMultipleImages(
	ID3D11Device* device, ID3D11DeviceContext* context, std::vector<W_STR>& filenames)
{
	int num_textures = filenames.size();
	HRESULT hr = S_OK;
	std::vector<ComPtr<ID3D11Texture2D>> src_textures(num_textures);

	for (int i = 0; i < num_textures; i++)
	{
		HRESULT hr = DirectX::CreateWICTextureFromFileEx(device,
			filenames[i].c_str(),
			0, D3D11_USAGE_STAGING,
			0, D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ,
			0, WIC_LOADER_DEFAULT,
			(ID3D11Resource**)src_textures[i].GetAddressOf(),
			nullptr);
	}

	D3D11_TEXTURE2D_DESC texElementDesc;
	src_textures[0]->GetDesc(&texElementDesc);

	D3D11_TEXTURE2D_DESC texArrayDesc;
	ZeroMemory(&texArrayDesc, sizeof(texArrayDesc));
	texArrayDesc.Width = texElementDesc.Width;
	texArrayDesc.Height = texElementDesc.Height;
	texArrayDesc.MipLevels = texElementDesc.MipLevels;
	texArrayDesc.ArraySize = num_textures;
	texArrayDesc.Format = texElementDesc.Format;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> texArray = 0;
	device->CreateTexture2D(&texArrayDesc, 0, texArray.GetAddressOf());

	for (UINT texElement = 0; texElement < num_textures; ++texElement)
	{
		// for each mipmap level...
		for (UINT mipLevel = 0; mipLevel < texElementDesc.MipLevels; ++mipLevel)
		{
			D3D11_MAPPED_SUBRESOURCE mappedTex2D;
			context->Map(src_textures[texElement].Get(), mipLevel, D3D11_MAP_READ, 0, &mappedTex2D);

			context->UpdateSubresource(texArray.Get(),
				D3D11CalcSubresource(mipLevel, texElement, texElementDesc.MipLevels),
				0, mappedTex2D.pData, mappedTex2D.RowPitch, mappedTex2D.DepthPitch);

			context->Unmap(src_textures[texElement].Get(), mipLevel);
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize = num_textures;

	ID3D11ShaderResourceView* texArraySRV = 0;
	device->CreateShaderResourceView(texArray.Get(), &viewDesc, &texArraySRV);

	texArray = nullptr;
	for (int i = 0; i < num_textures; i++) src_textures[i] = nullptr;

	return texArraySRV;
}


static ID3D11ShaderResourceView* CreateSRVFromSpriteSheet(
	ID3D11Device* device, ID3D11DeviceContext* context, W_STR filename, int num_rows, int num_cols)
{
	int num_textures = num_rows * num_cols;
	ComPtr<ID3D11Texture2D> srcTexture;
	ComPtr<ID3D11Texture2D> texArray;

	HRESULT result = DirectX::CreateWICTextureFromFileEx(
		device,
		context,
		filename.c_str(),
		0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
		DirectX::DX11::WIC_LOADER_FLAGS::WIC_LOADER_IGNORE_SRGB,
		(ID3D11Resource**)srcTexture.GetAddressOf(), nullptr	);

	// Create the texture array by specifying the texture dimensions and the number of textures in the array
	D3D11_TEXTURE2D_DESC textureArrayDesc;
	srcTexture->GetDesc(&textureArrayDesc);
	int tex_width = textureArrayDesc.Width / num_cols;
	int tex_height = textureArrayDesc.Height / num_rows;


	textureArrayDesc.ArraySize = num_textures;
	textureArrayDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	device->CreateTexture2D(&textureArrayDesc, NULL, texArray.GetAddressOf());

	// Copy each 64x64 image from the original texture to the corresponding slice in the texture array
	for (int i = 0; i < num_textures; i++)
	{
		D3D11_BOX sourceRegion;
		sourceRegion.left = (i % num_cols) * tex_width;
		sourceRegion.top = (i / num_cols) * tex_height;
		sourceRegion.front = 0;
		sourceRegion.right = sourceRegion.left + tex_width;
		sourceRegion.bottom = sourceRegion.top + tex_height;
		sourceRegion.back = 1;
		context->CopySubresourceRegion(texArray.Get(), D3D11CalcSubresource(0, i, 1), 0, 0, 0, srcTexture.Get(), 0, &sourceRegion);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = textureArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.MipLevels = textureArrayDesc.MipLevels;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize = num_textures;

	ID3D11ShaderResourceView* texArraySRV = 0;
	device->CreateShaderResourceView(texArray.Get(), &viewDesc, &texArraySRV);

	texArray = nullptr;
	srcTexture = nullptr;

	return texArraySRV;
}
