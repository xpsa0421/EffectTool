#include "Camera.h"
bool Camera::Init()
{
	return true;
}

void Camera::CreateViewMatrix(Vector pos, Vector target, Vector up)
{
	_pos = pos;
	_target = target;
	_up = up;
	_viewMat = Matrix::LookAtLH(_pos, _target, _up);
	Update();
}

void Camera::CreateProjMatrix(float n, float f, float fov, float aspect)
{
	_near = n;
	_far = f;
	_fov = fov;
	_aspect = aspect;
	_projMat = Matrix::PerspectiveFovLH(_fov, _aspect, _near, _far);
}

void Camera::UpdateProjMatrix(float aspect)
{
	_aspect = aspect;
	_projMat = Matrix::PerspectiveFovLH(_fov, _aspect, _near, _far);
}

bool Camera::Frame()
{
	if (s_input.GetKey(VK_PRIOR) == KEY_DOWN) //page up
	{
		_speed += 5.0f;
	}
	if (s_input.GetKey(VK_NEXT) == KEY_DOWN) //page down
	{
		_speed -= 5.0f;
	}
	if (s_input.GetKey('W') == KEY_HOLD)
	{
		Vector v = _look * _speed * g_secondPerFrame;
		_pos += v;
	}
	if (s_input.GetKey('S') == KEY_HOLD)
	{
		Vector v = _look * _speed * g_secondPerFrame;
		_pos += v;
	}
	if (s_input.GetKey('A') == KEY_HOLD)
	{
		Vector v = _right * -10.0f * g_secondPerFrame;
		_pos = _pos + v;
	}
	if (s_input.GetKey('D') == KEY_HOLD)
	{
		Vector v = _right * 10.0f * g_secondPerFrame;
		_pos = _pos + v;
	}
	if (s_input.GetKey('Q') == KEY_HOLD)
	{
		Vector v = _up * 10.0f * g_secondPerFrame;
		_pos = _pos + v;
	}
	if (s_input.GetKey('E') == KEY_HOLD)
	{
		Vector v = _up * -10.0f * g_secondPerFrame;
		_pos = _pos + v;
	}
	Vector up = { 0,1,0 };
	_viewMat = Matrix::LookAtLH(_pos, _target, _up);
	Update();
	return true;
}

void Camera::Update()
{
	_right.x = _viewMat._11;
	_right.y = _viewMat._21;
	_right.z = _viewMat._31;

	_up.x = _viewMat._12;
	_up.y = _viewMat._22;
	_up.z = _viewMat._32;

	_look.x = _viewMat._13;
	_look.y = _viewMat._23;
	_look.z = _viewMat._33;

	_right = Vector::Normalise(_right);
	_up = Vector::Normalise(_up);
	_look = Vector::Normalise(_look);
}
