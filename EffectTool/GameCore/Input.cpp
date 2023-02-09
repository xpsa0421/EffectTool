#include "Input.h"

Input::Input()
{
    wnd_active_ = true;
}

Input::~Input()
{
    Release();
}

DWORD Input::GetKey(DWORD key)
{
    return key_states_[key];
}

bool Input::Init()
{
    ZeroMemory(key_states_, sizeof(DWORD) * 256);
    GetCursorPos(&mouse_pos_);
    ScreenToClient(g_hWnd, &mouse_pos_);
    mouse_pos_prev_ = mouse_pos_;
    return true;
}

bool Input::Frame()
{
    // ���α׷� �����찡 ��Ŀ�� �Ǿ����� ������ ��ǲ�� ���� �ʴ´�
    if (GetForegroundWindow() != g_hWnd)
    {
        wnd_active_ = false;
        memset(key_states_, KEY_FREE, sizeof(key_states_));
        return false;
    }

    GetCursorPos(&mouse_pos_);
    ScreenToClient(g_hWnd, &mouse_pos_);
    
    // �����찡 �������� ��Ŀ�� �Ǿ��־��� ��쿡�� ���콺 ��ġ ��ȭ���� ����Ѵ�
    if (wnd_active_ == true)
    {
        mouse_offset_.x = mouse_pos_.x - mouse_pos_prev_.x;
        mouse_offset_.y = mouse_pos_.y - mouse_pos_prev_.y;
    }
    else wnd_active_ = true;

    for (int i = 0; i < 256; i++)
    {
        SHORT keyState = GetAsyncKeyState(i);

        if (keyState & 0x8000) // if pressed
        {
            if (key_states_[i] == KEY_FREE || key_states_[i] == KEY_UP)
                key_states_[i] = KEY_DOWN;
            else
                key_states_[i] = KEY_HOLD;
        }
        else
        {
            if (key_states_[i] == KEY_DOWN || key_states_[i] == KEY_HOLD)
                key_states_[i] = KEY_UP;
            else
                key_states_[i] = KEY_FREE;
        }
    }

    mouse_pos_prev_ = mouse_pos_;
    return true;
}

bool Input::Release()
{
    return true;
}

