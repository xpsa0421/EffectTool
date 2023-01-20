#pragma once
#include "Std.h"
enum KeyState
{
	KEY_FREE = 0,
	KEY_UP,
	KEY_DOWN,
	KEY_HOLD
};

class Input : public Singleton<Input>
{
private:
	friend class Singleton<Input>;
private:
	bool	_isWndActive;
	DWORD	_keyStates[256];
public:
	POINT	_mousePos;
	POINT	_mousePosPrev;
	POINT	_mouseOffset;
public:
	bool	Init();
	bool	Frame();
	bool	Release();
public:
	DWORD	GetKey(DWORD key);
private:
	Input();
public:
	~Input();
};

#define s_input Input::GetInstance()