#include "GameCore.h"

HRESULT GameCore::CreateDXResource()
{
	writer_.Init();
	IDXGISurface1* backBuffer;
	swapchain_->GetBuffer(0, __uuidof(IDXGISurface1),
		(void**)&backBuffer);
	writer_.Set(backBuffer);
	backBuffer->Release();
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
	ID3D11ShaderResourceView* pSRVs[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	ID3D11RenderTargetView* pRTVs[16]	= { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	ID3D11DepthStencilView* pDSV		= NULL;
	ID3D11Buffer* pBuffers[16]			= { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	ID3D11SamplerState* pSamplers[16]	= { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	UINT StrideOffset[16]				= { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	immediate_context_->VSSetShaderResources(0, 16, pSRVs);
	immediate_context_->PSSetShaderResources(0, 16, pSRVs);
}
bool GameCore::CoreInit()
{
	Device::Init();
	DxState::CreateStates(device_);
	s_texManager.SetDevice(device_, immediate_context_);
	s_shaderManager.SetDevice(device_);

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
		immediate_context_->RSSetState(DxState::g_RSWireFrame);
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
	
	immediate_context_->OMSetRenderTargets(1, &rtv_, dsv_);
	immediate_context_->ClearRenderTargetView(rtv_, color);
	immediate_context_->ClearDepthStencilView(dsv_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	immediate_context_->PSSetSamplers(0, 1, &DxState::g_SSWrap);
	immediate_context_->RSSetViewports(1, &viewport_);
	immediate_context_->RSSetState(DxState::g_RSSolid);
	immediate_context_->OMSetBlendState(DxState::g_BSAlpha, 0, -1);
	immediate_context_->OMSetDepthStencilState(DxState::g_DSDepthEnable, 0xff);

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
	DxState::Release();
	Device::Release();
	writer_.Release();
	if (cam_) delete cam_;
	
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