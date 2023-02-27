#include "GameCore.h"

HRESULT GameCore::CreateDXResource()
{
	writer_.Init();
	ComPtr<IDXGISurface1> back_buffer = nullptr;
	swapchain_->GetBuffer(0, __uuidof(IDXGISurface1),
		(void**)&back_buffer);
	writer_.Set(back_buffer.Get());
	back_buffer = nullptr;

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
	global_timer.Init();
	CreateDXResource();

	Init();

#ifdef USE_IMGUI
	ImGui_ImplWin32_Init(window_->_hWnd);
	ImGui_ImplDX11_Init(device_.Get(), device_context_.Get());
#endif // USE_IMGUI

	return true;
}

bool GameCore::CoreFrame()
{
#ifdef USE_IMGUI
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#endif // USE_IMGUI
	if (window_->_resizeWindow == true) 
	{
		ResizeWindow(window_->_clientWidth, window_->_clientHeight);
	}
	s_input.Frame();
	global_timer.Frame();
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
	//if (cam_) PrintDebugInfo();
#endif // DEBUG

	CorePostRender();

	return true;
}

bool GameCore::CorePreRender()
{
	float color[4] = { 0.45f, 0.45f, 0.45f, 1.0f };
	
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
	global_timer.Render();
	writer_.Render();

#ifdef USE_IMGUI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif // USE_IMGUI
	
	swapchain_->Present(0, 0);
	return true;
}

bool GameCore::CoreRelease()
{
#ifdef USE_IMGUI
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif // USE_IMGUI

	Release();
	if (cam_) delete cam_;
	writer_.Release();
	DxState.Release();
	shader_manager.Release();
	texture_manager.Release();
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

bool GameCore::RunMFC()
{
	if (!CoreFrame() || !CoreRender())
	{
		game_active_ = false;
		return false;
	}
	return true;
}

bool GameCore::SetWindow(HINSTANCE hInstance, const WCHAR* title, UINT width, UINT height)
{
	window_ = new Window;
	window_->SetWindow(hInstance, title, width, height);

#ifdef USE_IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
#endif // USE_IMGUI

	return true;
}

void GameCore::ResizeWindow(UINT width, UINT height) 
{
	if (swapchain_ == nullptr) {
		return;
	} 
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