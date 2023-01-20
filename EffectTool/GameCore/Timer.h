#pragma once
#include "Std.h"
#pragma comment(lib,"winmm.lib")
class Timer
{
public:
	float			_timer;
	float			_elapseTime;
	DWORD			_prevTime;
	LARGE_INTEGER	_frequency;
	LARGE_INTEGER	_prevPfCount;
	LARGE_INTEGER	_currentPfCount;
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
};

