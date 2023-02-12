#include "GameCore.h"

HRESULT GameCore::CreateDXResource()
{
	writer_.Init();
	ComPtr<IDXGISurface1> back_buffer = nullptr;
	swapchain_->GetBuffer(0, __uuidof(IDXGISurface1),
		(void**)&back_buffer);
	writer_.Set(back_buffer.Get());

	return S_OK;
}

HRESULT GameCore::DeleteDXResource()
{
	HRESULT hr;
	if (FAILED(hr = writer_.DeleteDXResource())) return hr;
	return S_OK;
}

void GameCore::ClearD3D11DeviceContext()
{
	ID3D11DepthStencilView* DSV			= NULL;
	ID3D11ShaderResourceView* SRVs[16]	= { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	ID3D11RenderTargetView* RTVs[16]	= { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	ID3D11Buffer* buffers[16]			= { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	ID3D11SamplerState* samplers[16]	= { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	UINT strideOffset[16]				= { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	device_context_->VSSetShaderResources(0, 16, SRVs);
	device_context_->PSSetShaderResources(0, 16, SRVs);
}

bool GameCore::CoreInit()
{
	Device::Init();
	
	DxState.Init();
	DxState.SetDevice(device_.Get(), device_context_.Get());
	DxState.CreateStates();

	texture_manager.Init();
	texture_manager.SetDevice(device_.Get(), device_context_.Get());
	
	shader_manager.Init();
	shader_manager.SetDevice(device_.Get());
	

	s_input.Init();
	s_gameTimer.Init();
	CreateDXResource();

	return Init();
}

bool GameCore::CoreFrame()
{
	if (window_->_resizeWindow == true) 
	{
		ResizeWindow(window_->_clientWidth, window_->_clientHeight);
	}
	s_input.Frame();
	s_gameTimer.Frame();
	writer_.Frame();
	ClearD3D11DeviceContext();

	if (cam_) cam_->Frame();

	return Frame();
}

bool GameCore::CoreRender()
{
	CorePreRender();
#ifdef _DEBUG
	if (s_input.GetKey(VK_F1) == KEY_HOLD)
	{
		DxState.ApplyRasterizerState(L"RS_wireframe");
	}
#endif // DEBUG
	Render();
#ifdef _DEBUG
	if (cam_) PrintDebugInfo();
#endif // DEBUG

	CorePostRender();

	return true;
}

bool GameCore::CorePreRender()
{
	float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	
	device_context_->OMSetRenderTargets(1, rtv_.GetAddressOf(), dsv_.Get());
	device_context_->ClearRenderTargetView(rtv_.Get(), color);
	device_context_->ClearDepthStencilView(dsv_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	device_context_->RSSetViewports(1, &viewport_);

	DxState.ApplySamplerState(L"SS_wrap");
	DxState.ApplyRasterizerState(L"RS_solid");
	DxState.ApplyBlendState(L"BS_alpha", 0, -1);
	DxState.ApplyDepthStencilState(L"DS_depth_enable", 0xff);

	return true;
}

bool GameCore::CorePostRender()
{
	s_gameTimer.Render();
	writer_.Render();
	swapchain_->Present(0, 0);

	return true;
}

bool GameCore::CoreRelease()
{
	Release();
	DxState.Release();
	writer_.Release();
	if (cam_) delete cam_;
	delete window_;
	Device::Release();

	return true;
}

bool GameCore::Run()
{
	CoreInit();
	while(game_active_)
	{
		if (window_->Run() == true)
		{
			CoreFrame();
			CoreRender();
		}
		else {
			game_active_ = false;
		}
	}
	CoreRelease();
	return true;
}

bool GameCore::SetWindow(HINSTANCE hInstance, const WCHAR* title, UINT width, UINT height)
{
	window_ = new Window;
	window_->SetWindow(hInstance, title, width, height);
	return true;
}

void GameCore::ResizeWindow(UINT width, UINT height) 
{
	//if (swapchain_ == nullptr) {
	//	return;
	//} mfc 사용 시 활성화
	Device::ResizeWindow(width, height);
	window_->_resizeWindow = false;
}

void GameCore::PrintDebugInfo()
{
	W_STR text = mtw("Camera position:\n") + std::to_wstring(cam_->pos_.x) +
		L"   " + std::to_wstring(cam_->pos_.y) +
		L"   " + std::to_wstring(cam_->pos_.z);
	writer_.Draw(10, 30, text);

	T_STR str;
	TCHAR pBuffer[256];
	memset(pBuffer, 0, sizeof(TCHAR) * 256);
	_stprintf_s(pBuffer, _T("Look:%10.4f,%10.4f,%10.4f \n"), cam_->look_.x, cam_->look_.y, cam_->look_.z);
	str += pBuffer;

	memset(pBuffer, 0, sizeof(TCHAR) * 256);
	_stprintf_s(pBuffer, _T("Up:%10.4f,%10.4f,%10.4f \n"), cam_->up_.x, cam_->up_.y, cam_->up_.z);
	str += pBuffer;

	memset(pBuffer, 0, sizeof(TCHAR) * 256);
	_stprintf_s(pBuffer, _T("Right:%10.4f,%10.4f,%10.4f "), cam_->right_.x, cam_->right_.y, cam_->right_.z);
	str += pBuffer;

	writer_.Draw(g_rectClient.right-320, g_rectClient.bottom-100, str);
}