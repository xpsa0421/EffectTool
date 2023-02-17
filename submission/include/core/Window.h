#pragma once
#include "Std.h"

class Window
{
public:
	HINSTANCE		_hInstance;
	HWND			_hWnd;
	RECT			_rtWindow;
	RECT			_rtClient;
	DWORD			_wndStyle;
	UINT			_clientWidth;
	UINT			_clientHeight;
	bool			_resizeWindow = false;
public:
	RECT			GetClientRtSize();
	ATOM			MyRegisterClass();
	void			CenterWindow();
	void			SethWnd(HWND hWnd);
	bool			SetWindow(HINSTANCE hInst, const WCHAR* title, UINT width, UINT height);
	bool			Run();
	BOOL			InitInstance(const WCHAR* title, UINT width, UINT height);
public:
	virtual LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	Window();
};	

