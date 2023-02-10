#pragma once
#include "Std.h"

class Device 
{
public:
	ComPtr<ID3D11Device>			device_;
	ComPtr<ID3D11DeviceContext>		device_context_;
	ComPtr<IDXGIFactory>			factory_; 
	ComPtr<IDXGISwapChain>			swapchain_;
	ComPtr<ID3D11RenderTargetView>	rtv_;
	ComPtr<ID3D11DepthStencilView>	dsv_;
	D3D11_VIEWPORT					viewport_;

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
	void			CreateViewport();
public:
	HRESULT			ResizeWindow(UINT width, UINT height);
};

