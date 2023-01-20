#include "Timer.h"

bool Timer::Init()
{
    _timer = 0.0f;
    _elapseTime = 10.0f;
    _prevTime = timeGetTime();
    QueryPerformanceFrequency(&_frequency);
    QueryPerformanceCounter(&_prevPfCount);
    return true;
}

bool Timer::Frame()
{
    QueryPerformanceCounter(&_currentPfCount);
    _elapseTime = (float)(_currentPfCount.QuadPart - _prevPfCount.QuadPart) / (float)_frequency.QuadPart;
    _timer += _elapseTime;
    _prevPfCount = _currentPfCount;
    return true;
}

bool Timer::Render()
{
    return true;
}

bool Timer::Release()
{
    return true;
}
