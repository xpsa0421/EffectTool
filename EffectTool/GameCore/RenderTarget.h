#pragma once
#include "Device.h"

class RenderTarget
{
public:
	ID3D11RenderTargetView*			_renderTargetView;
	ID3D11DepthStencilView*			dsv_;
	ID3D11ShaderResourceView*		_shaderResourceView;
	ID3D11ShaderResourceView*		_depthSRV;
	ID3D11Texture2D*				_texture2D;
	D3D11_DEPTH_STENCIL_VIEW_DESC	_depthStencilDesc;
	D3D11_TEXTURE2D_DESC			_textureDesc;
	D3D11_VIEWPORT					viewport_;
public:
	ID3D11RenderTargetView*			_oldRenderTarget;
	ID3D11DepthStencilView*			_oldDepthStencil;
	D3D11_VIEWPORT					_oldViewPort[D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];
public:
	bool Create(ID3D11Device* device, FLOAT width = 1000.0f, FLOAT height = 700.0f);
	bool Begin(ID3D11DeviceContext* context);
	void End(ID3D11DeviceContext* context);
	bool Release();
};

