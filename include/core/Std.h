#pragma once

//-----------------------------------------------------------------------------
// Standard includes
//-----------------------------------------------------------------------------
#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgidebug.h>
#include <codecvt>
#include <tchar.h>
#include <wrl.h>
#include <DirectXMath.h>


//-----------------------------------------------------------------------------
// Standard libraries
//-----------------------------------------------------------------------------
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
extern HWND		g_hWnd;
extern RECT		g_rectClient;
extern float	g_timer;
extern float	g_spf;
extern UINT		g_fps;


//-----------------------------------------------------------------------------
// Type defines
//-----------------------------------------------------------------------------
typedef std::basic_string<TCHAR>				T_STR;
typedef std::basic_string<wchar_t>				W_STR;
typedef std::basic_string<char>					C_STR;
typedef std::string								STR;
typedef std::vector<std::basic_string<TCHAR>>	TSTR_VECTOR;
typedef std::vector<DWORD>						DWORD_VECTOR;
typedef std::vector<float>						FLOAT_VECTOR;


//-----------------------------------------------------------------------------
// Namespaces
//-----------------------------------------------------------------------------
using namespace Microsoft::WRL;
using namespace DirectX;


//-----------------------------------------------------------------------------
// Standard classes
//-----------------------------------------------------------------------------
template<class T> class Singleton
{
public:
	static T& GetInstance()
	{
		static T theSingleInstance;
		return theSingleInstance;
	}
};


//-----------------------------------------------------------------------------
// Standard functions
//-----------------------------------------------------------------------------
static W_STR mtw(STR str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.from_bytes(str);
}

static STR wtm(W_STR str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.to_bytes(str);
}

