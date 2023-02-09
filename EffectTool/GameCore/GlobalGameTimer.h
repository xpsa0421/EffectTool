#pragma once
#include "Timer.h"

class GlobalGameTimer : public Timer, public Singleton<GlobalGameTimer>
{
private:
	float	fps_timer_;
	UINT	fps_counter_;
public:
	virtual bool Frame();
};

#define s_gameTimer GlobalGameTimer::GetInstance()