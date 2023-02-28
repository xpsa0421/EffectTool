#include "Window.h"
#include "Device.h" 
#include "ImGuiLayer.h"

HWND	g_hWnd;
RECT	g_rectClient;
Window* g_window = nullptr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
#ifdef USE_IMGUI
	if (ImGuiLayer::MsgProcHandler(hWnd, message, wParam, lParam))
		return true;
#endif // USE_IMGUI
	return g_window->MsgProc(hWnd, message, wParam, lParam);
}

Window::Window()
{
	g_window = this;
}

LRESULT Window::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
	{
		if (SIZE_MINIMIZED != wParam)
		{
			RECT preClient = _rtClient;
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
			GetWindowRect(hWnd, &_rtWindow);
			GetClientRect(hWnd, &_rtClient);

			g_rectClient = _rtClient;
			_clientWidth = _rtClient.right - _rtClient.left;
			_clientHeight = _rtClient.bottom - _rtClient.top;
			
			if (preClient == _rtClient)
			{
				break;
			}
			_resizeWindow = true;
		}
	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
BOOL Window::InitInstance(const WCHAR* title, UINT width, UINT height)
{
	_wndStyle = WS_OVERLAPPEDWINDOW;
	RECT rc = { 0,0, width, height };
	AdjustWindowRect(&rc, _wndStyle, FALSE);
	HWND hWnd = CreateWindowW(L"window", title, _wndStyle, 0, 0,
							  rc.right - rc.left, rc.bottom - rc.top,
							  nullptr, nullptr, _hInstance, nullptr);
	if (!hWnd) 
	{
		return FALSE;
	}
	SethWnd(hWnd);
	ShowWindow(hWnd, SW_SHOW);
	CenterWindow();

	return TRUE;
}

void Window::SethWnd(HWND hWnd)
{
	_hWnd = hWnd;
	g_hWnd = hWnd;
	GetWindowRect(hWnd, &_rtWindow);
	GetClientRect(hWnd, &_rtClient);
	g_rectClient = _rtClient;
	_clientWidth = _rtClient.right - _rtClient.left;
	_clientHeight = _rtClient.bottom - _rtClient.top;
}

RECT Window::GetClientRtSize()
{
	GetClientRect(_hWnd, &_rtClient);
	return _rtClient;
}

void Window::CenterWindow()
{
	UINT screenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	UINT screenHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	UINT cx, cy;
	cx = (screenWidth - (_rtWindow.right - _rtWindow.left)) * 0.5f;
	cy = (screenHeight - (_rtWindow.bottom - _rtWindow.top)) * 0.5f;

	MoveWindow(_hWnd, cx, cy, _rtWindow.right - _rtWindow.left,
			   _rtWindow.bottom - _rtWindow.top, true);
}


ATOM Window::MyRegisterClass()
{
	WNDCLASSEXW  wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hInstance = _hInstance;
	wcex.hbrBackground = CreateSolidBrush(RGB(1, 1, 1));
	wcex.lpszClassName = L"window";
	wcex.lpfnWndProc = WndProc; 
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	return RegisterClassEx(&wcex);
}

bool Window::SetWindow(HINSTANCE hInst, const WCHAR* title, UINT width, UINT height)
{
	_hInstance = hInst;
	WORD ret = MyRegisterClass();
	if (!InitInstance(title, width, height)) 
	{
		return false;
	}
	return true;
}

bool Window::Run()
{
	MSG msg = { 0, };
	while (WM_QUIT != msg.message) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
			return true; 
		}
	}
	return	false;
}


