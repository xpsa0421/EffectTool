#include "Input.h"

Input::Input()
{
    _isWndActive = true;
}

Input::~Input()
{
    Release();
}

DWORD Input::GetKey(DWORD key)
{
    return _keyStates[key];
}

bool Input::Init()
{
    ZeroMemory(_keyStates, sizeof(DWORD) * 256);
    GetCursorPos(&_mousePos);
    ScreenToClient(g_hWnd, &_mousePos);
    _mousePosPrev = _mousePos;
    return true;
}

bool Input::Frame()
{
    // ���α׷� �����찡 ��Ŀ�� �Ǿ����� ������ ��ǲ�� ���� �ʴ´�
    if (GetForegroundWindow() != g_hWnd)
    {
        _isWndActive = false;
        memset(_keyStates, KEY_FREE, sizeof(_keyStates));
        return false;
    }

    GetCursorPos(&_mousePos);
    ScreenToClient(g_hWnd, &_mousePos);
    
    // �����찡 �������� ��Ŀ�� �Ǿ��־��� ��쿡�� ���콺 ��ġ ��ȭ���� ����Ѵ�
    if (_isWndActive == true)
    {
        _mouseOffset.x = _mousePos.x - _mousePosPrev.x;
        _mouseOffset.y = _mousePos.y - _mousePosPrev.y;
    }
    else _isWndActive = true;

    for (int i = 0; i < 256; i++)
    {
        SHORT keyState = GetAsyncKeyState(i);

        if (keyState & 0x8000)
        {
            if (_keyStates[i] == KEY_FREE || _keyStates[i] == KEY_UP)
            {
                _keyStates[i] = KEY_DOWN;
            }
            else
            {
                _keyStates[i] = KEY_HOLD;
            }
        }
        else
        {
            if (_keyStates[i] == KEY_DOWN || _keyStates[i] == KEY_HOLD)
            {
                _keyStates[i] = KEY_UP;
            }
            else
            {
                _keyStates[i] = KEY_FREE;
            }
        }
    }
    return true;
}

bool Input::Release()
{
    return true;
}

