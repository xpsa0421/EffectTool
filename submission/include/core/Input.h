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
	bool	wnd_active_;
	DWORD	key_states_[256];
public:
	POINT	mouse_pos_;
	POINT	mouse_pos_prev_;
	POINT	mouse_offset_;
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