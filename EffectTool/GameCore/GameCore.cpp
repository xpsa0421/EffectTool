#include "GameCore.h"

HRESULT GameCore::CreateDXResource()
{
	_writer.Init();
	IDXGISurface1* backBuffer;
	_swapChain->GetBuffer(0, __uuidof(IDXGISurface1),
		(void**)&backBuffer);
	_writer.Set(backBuffer);
	backBuffer->Release();
	return S_OK;
}

HRESULT GameCore::DeleteDXResource()
{
	HRESULT hr;
	if (FAILED(hr = _writer.DeleteDXResource())) return hr;
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
	_immediateContext->VSSetShaderResources(0, 16, pSRVs);
	_immediateContext->PSSetShaderResources(0, 16, pSRVs);
}
bool GameCore::CoreInit()
{
	Device::Init();
	DxState::CreateStates(_device);
	s_texManager.SetDevice(_device, _immediateContext);
	s_shaderManager.SetDevice(_device);

	s_input.Init();
	s_gameTimer.Init();
	CreateDXResource();

	return Init();
}

bool GameCore::CoreFrame()
{
	if (_window->_resizeWindow == true) 
	{
		ResizeWindow(_window->_clientWidth, _window->_clientHeight);
	}
	s_input.Frame();
	s_gameTimer.Frame();
	_writer.Frame();
	ClearD3D11DeviceContext();

	if (_cam) _cam->Frame();

	return Frame();
}

bool GameCore::CoreRender()
{
	CorePreRender();
#ifdef _DEBUG
	if (s_input.GetKey(VK_F1) == KEY_HOLD)
	{
		_immediateContext->RSSetState(DxState::g_RSWireFrame);
	}
#endif // DEBUG
	Render();
#ifdef _DEBUG
	if (_cam) PrintDebugInfo();
#endif // DEBUG

	CorePostRender();

	return true;
}

bool GameCore::CorePreRender()
{
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	
	_immediateContext->OMSetRenderTargets(1, &_rtv, _dsv);
	_immediateContext->ClearRenderTargetView(_rtv, color);
	_immediateContext->ClearDepthStencilView(_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	_immediateContext->PSSetSamplers(0, 1, &DxState::g_SSWrap);
	_immediateContext->RSSetViewports(1, &_viewport);
	_immediateContext->RSSetState(DxState::g_RSSolid);
	_immediateContext->OMSetBlendState(DxState::g_BSAlpha, 0, -1);
	_immediateContext->OMSetDepthStencilState(DxState::g_DSDepthEnable, 0xff);

	return true;
}

bool GameCore::CorePostRender()
{
	s_gameTimer.Render();
	_writer.Render();
	_swapChain->Present(0, 0);

	return true;
}

bool GameCore::CoreRelease()
{
	Release();
	DxState::Release();
	Device::Release();
	_writer.Release();
	if (_cam) delete _cam;
	
	return true;
}

bool GameCore::Run()
{
	CoreInit();
	while(_isGameRun)
	{
		if (_window->Run() == true)
		{
			CoreFrame();
			CoreRender();
		}
		else {
			_isGameRun = false;
		}
	}
	CoreRelease();
	return true;
}

bool GameCore::SetWindow(HINSTANCE hInstance, const WCHAR* title, UINT width, UINT height)
{
	_window = new Window;
	_window->SetWindow(hInstance, title, width, height);
	return true;
}

void GameCore::ResizeWindow(UINT width, UINT height) 
{
	//if (_swapChain == nullptr) {
	//	return;
	//} mfc 사용 시 활성화
	Device::ResizeWindow(width, height);
	_window->_resizeWindow = false;
}

void GameCore::PrintDebugInfo()
{
	W_STR text = mtw("Camera position:\n") + std::to_wstring(_cam->_pos.x) +
		L"   " + std::to_wstring(_cam->_pos.y) +
		L"   " + std::to_wstring(_cam->_pos.z);
	_writer.Draw(10, 30, text);

	T_STR str;
	TCHAR pBuffer[256];
	memset(pBuffer, 0, sizeof(TCHAR) * 256);
	_stprintf_s(pBuffer, _T("Look:%10.4f,%10.4f,%10.4f \n"), _cam->_look.x, _cam->_look.y, _cam->_look.z);
	str += pBuffer;

	memset(pBuffer, 0, sizeof(TCHAR) * 256);
	_stprintf_s(pBuffer, _T("Up:%10.4f,%10.4f,%10.4f \n"), _cam->_up.x, _cam->_up.y, _cam->_up.z);
	str += pBuffer;

	memset(pBuffer, 0, sizeof(TCHAR) * 256);
	_stprintf_s(pBuffer, _T("Right:%10.4f,%10.4f,%10.4f "), _cam->_right.x, _cam->_right.y, _cam->_right.z);
	str += pBuffer;

	_writer.Draw(g_rectClient.right-320, g_rectClient.bottom-100, str);
}