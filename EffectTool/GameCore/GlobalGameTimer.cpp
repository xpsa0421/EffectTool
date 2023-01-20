#include "GlobalGameTimer.h"

float   g_gameTimer = 0.0f;
float   g_secondPerFrame = 0.0f;
UINT    g_fps = 0.0f;

bool GlobalGameTimer::Frame()
{
    Timer::Frame();
    _fpsCounter++;
    _fpsTimer += _elapseTime;
    if (_fpsTimer >= 1.0f)
    {
        g_fps = _fpsCounter;
        _fpsCounter = 0;
        _fpsTimer -= 1.0f;
    }
    g_gameTimer = _timer;
    g_secondPerFrame = _elapseTime;
    return true;
}
