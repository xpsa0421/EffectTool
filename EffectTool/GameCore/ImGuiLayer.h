#pragma once
#include "Std.h"

class ImGuiLayer
{
public:
	static void	Init(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context);
	static void	Begin();
	static void	End();
	static void	Release();
	static bool	MsgProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
