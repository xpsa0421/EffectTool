#pragma once
#include "Device.h"
#include "Window.h"
#include "Input.h"
#include "Writer.h"
#include "DxState.h"
#include "GlobalGameTimer.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "Camera.h"

class GameCore : public Device
{
private:
	bool			game_active_	= true;
	Window*			window_			= nullptr;

protected:
	Writer			writer_;
	Camera*			cam_;

private:
	bool			CoreInit();
	bool			CoreFrame();
	bool			CoreRender();
	bool			CorePreRender();
	bool			CorePostRender();
	bool			CoreRelease();

protected:
	virtual bool	Init() { return true; };
	virtual bool	Frame() { return true; };
	virtual bool	Render() { return true; };
	virtual bool	Release() { return true; };

	void			ResizeWindow(UINT width, UINT height);
	HRESULT			CreateDXResource() override;
	HRESULT			DeleteDXResource() override;
	void			ClearD3D11DeviceContext();
	void			PrintDebugInfo();

public:
	bool			Run();
	bool			SetWindow(HINSTANCE hInstance, const WCHAR* Title, UINT width, UINT height);

};