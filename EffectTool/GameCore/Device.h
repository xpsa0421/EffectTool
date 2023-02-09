#pragma once
#include "Std.h"

class Device 
{
public:
	ID3D11Device*			device_				= nullptr;
	ID3D11DeviceContext*	immediate_context_	= nullptr;
	IDXGIFactory*			factory_			= nullptr; 
	IDXGISwapChain*			swapchain_			= nullptr;
	ID3D11RenderTargetView* rtv_				= nullptr;
	ID3D11DepthStencilView* dsv_				= nullptr;
	D3D11_VIEWPORT			viewport_;

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

