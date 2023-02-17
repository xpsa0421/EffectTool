#pragma once
#include "Std.h"
#define DxState ID3D11State::GetInstance()

class ID3D11State : public Singleton<ID3D11State>
{
private:
	friend class Singleton<ID3D11State>;
	
	ComPtr<ID3D11Device>				device_;
	ComPtr<ID3D11DeviceContext>			device_context_;

	std::map<W_STR, ID3D11DeviceChild*> states_;

public:
	//-----------------------------------------------------------------------------
	// Sampler states
	//-----------------------------------------------------------------------------
	ComPtr<ID3D11SamplerState>		SS_smooth_;
	ComPtr<ID3D11SamplerState>		SS_wrap_;
	ComPtr<ID3D11SamplerState>		SS_mirror_;
	ComPtr<ID3D11SamplerState>		SS_border_;

	//-----------------------------------------------------------------------------
	// Blend states
	//-----------------------------------------------------------------------------
	ComPtr<ID3D11BlendState>		BS_alpha_;
	ComPtr<ID3D11BlendState>		BS_no_apha_;
	ComPtr<ID3D11BlendState>		BS_one_zero_;
	ComPtr<ID3D11BlendState>		BS_dual_source_blend_;
	ComPtr<ID3D11BlendState>		BS_dual_source_no_blend_;

	//-----------------------------------------------------------------------------
	// Rasterizer states
	//-----------------------------------------------------------------------------
	ComPtr<ID3D11RasterizerState>	RS_wireframe_;
	ComPtr<ID3D11RasterizerState>	RS_solid_;

	//-----------------------------------------------------------------------------
	// Depth Stencil states
	//-----------------------------------------------------------------------------
	ComPtr<ID3D11DepthStencilState> DS_depth_disable_;
	ComPtr<ID3D11DepthStencilState> DS_depth_enable_no_write_;
	ComPtr<ID3D11DepthStencilState> DS_depth_enable_;
	ComPtr<ID3D11DepthStencilState> DS_depth_greater_;

public:
	void	Init();
	void	SetDevice(ID3D11Device* device, ID3D11DeviceContext* context);
	bool	CreateStates();
	bool	Release();

	void	ApplySamplerState(W_STR state_name, UINT start_slot = 0, UINT num_samplers = 1);
	void	ApplyBlendState(W_STR state_name, const FLOAT blend_factor[] = 0, UINT sample_mask = 0xffffffff);
	void	ApplyRasterizerState(W_STR state_name);
	void	ApplyDepthStencilState(W_STR state_name, UINT stencil_ref = 0x01);
};
