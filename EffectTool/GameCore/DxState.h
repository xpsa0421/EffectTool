#pragma once
#include "Std.h"
#include <d3d11.h> // TODO to be added to Std.h

class DxState
{
public:
	//-----------------------------------------------------------------------------
	// Sampler states
	//-----------------------------------------------------------------------------
	static ID3D11SamplerState*		g_SSSmooth;
	static ID3D11SamplerState*		g_SSWrap;
	static ID3D11SamplerState*		g_SSMirror;
	static ID3D11SamplerState*		g_SSBorder;

	//-----------------------------------------------------------------------------
	// Blend states
	//-----------------------------------------------------------------------------
	static ID3D11BlendState*		g_BSAlpha;
	static ID3D11BlendState*		g_BSNoAlpha;
	static ID3D11BlendState*		g_BSOneZero;

	//-----------------------------------------------------------------------------
	// Rasterizer states
	//-----------------------------------------------------------------------------
	static ID3D11RasterizerState*	g_RSWireFrame;
	static ID3D11RasterizerState*	g_RSSolid;

	//-----------------------------------------------------------------------------
	// Depth Stencil states
	//-----------------------------------------------------------------------------
	static ID3D11DepthStencilState* g_DSDepthDisable;
	static ID3D11DepthStencilState* g_DSDepthEnable;
	static ID3D11DepthStencilState* g_DSDepthGreater;

public:
	static bool CreateStates(ID3D11Device* device);
	static bool Release();

	static void ApplySamplerState(ID3D11DeviceContext* context,
									ID3D11SamplerState* samplerState, 
									UINT startSlot = 0,
									UINT numSamplers = 1);
	static void ApplyBlendState(ID3D11DeviceContext* context,
									ID3D11BlendState* blendState,
									const FLOAT blendFactor[] = 0,
									UINT sampleMask = 0xffffffff);
	static void ApplyRasterizerState(ID3D11DeviceContext* context,
									ID3D11RasterizerState* rasterizerState);
	static void ApplyDepthStencilState(ID3D11DeviceContext* context,
									ID3D11DepthStencilState* depthStencilState,
									UINT stencilRef = 0x01);
};
