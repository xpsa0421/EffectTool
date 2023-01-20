#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include "Std.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
class Device 
{
public:
	ID3D11Device*			_device				= nullptr;
	ID3D11DeviceContext*	_immediateContext	= nullptr;
	IDXGIFactory*			_giFactory			= nullptr; 
	IDXGISwapChain*			_swapChain			= nullptr;
	ID3D11RenderTargetView* _rtv				= nullptr;
	ID3D11DepthStencilView* _dsv				= nullptr;
	D3D11_VIEWPORT			_viewport;

public:
	bool			Init();	
	bool			Release(); 
	virtual HRESULT	CreateDXResource();
	virtual HRESULT	DeleteDXResource();
public: 
	HRESULT			CreateDevice();
	HRESULT			CreateFactory();
	HRESULT			CreateSwapChain();
	HRESULT			CreateRenderTargetView();
	HRESULT			CreateDepthStencilView();
	void			CreateViewPort();
public:
	HRESULT			ResizeWindow(UINT width, UINT height);
};

