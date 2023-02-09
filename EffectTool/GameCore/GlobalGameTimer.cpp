#include "GlobalGameTimer.h"

float   g_timer = 0.0f;
float   g_spf = 0.0f;
UINT    g_fps = 0.0f;

bool GlobalGameTimer::Frame()
{
    Timer::Frame();
    fps_counter_++;
    fps_timer_ += _elapseTime;
    if (fps_timer_ >= 1.0f)
    {
        g_fps = fps_counter_;
        fps_counter_ = 0;
        fps_timer_ -= 1.0f;
    }
    g_timer = _timer;
    g_spf = _elapseTime;
    return true;
}
