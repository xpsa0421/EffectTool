#pragma once
#include "Device.h"

class RenderTarget
{
public:
	bool Create(ID3D11Device* device, FLOAT width = 600, FLOAT height = 800.0f);
	bool Begin();
	void End();
	bool Release();

	void Resize(FLOAT width, FLOAT height);
	void SetClearColor(const FLOAT color[4]);

private:
	ComPtr<ID3D11DeviceContext>			device_context_;
	ComPtr<ID3D11RenderTargetView>		rtv_;
	ComPtr<ID3D11DepthStencilView>		dsv_;
	ComPtr<ID3D11ShaderResourceView>	depth_srv_;
	ComPtr<ID3D11Texture2D>				texture2D_;
	D3D11_VIEWPORT						viewport_;

	ID3D11RenderTargetView*		rtv_old_;
	ID3D11DepthStencilView*		dsv_old_;
	D3D11_VIEWPORT						viewport_old_[D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];

	FLOAT clear_color_[4];

public:
	ComPtr<ID3D11ShaderResourceView>	srv_;
};

