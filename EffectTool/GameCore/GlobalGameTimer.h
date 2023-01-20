#pragma once
#include "Timer.h"
class GlobalGameTimer : public Timer, public Singleton<GlobalGameTimer>
{
private:
	float	_fpsTimer;
	UINT	_fpsCounter;
public:
	virtual bool Frame();
};

#define s_gameTimer GlobalGameTimer::GetInstance()