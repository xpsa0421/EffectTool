#include "Camera.h"

Camera::Camera()
{
	Init();
}

bool Camera::Init()
{
	_roll = _pitch = _yaw = 0.0;
	return true;
}

void Camera::CreateViewMatrix(Vector pos, Vector target, Vector up)
{
	_pos = pos;
	_target = target;
	_up = up;
	_viewMat = Matrix::LookAtLH(_pos, _target, _up);

	Matrix mInvView = Matrix::Inverse(_viewMat);
	Vector pZBasis = Vector(mInvView._31, mInvView._32, mInvView._33);
	float fLen = sqrtf(pZBasis.z * pZBasis.z + pZBasis.x * pZBasis.x);

	_yaw = atan2f(pZBasis.x, pZBasis.z);
	_pitch = -atan2f(pZBasis.y, fLen);
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
	// view direction control
	if (s_input.GetKey(VK_LBUTTON) == KEY_HOLD)
	{
		_yaw += s_input._mouseOffset.x * 0.002f;
		_pitch += s_input._mouseOffset.y * 0.002f;
	}

	// move speed control
	if (s_input.GetKey(VK_SPACE) == KEY_HOLD)
	{
		_speed += g_secondPerFrame * 100.0f;
		_speed = min(100.0f, _speed);
	}
	else
	{
		_speed -= g_secondPerFrame * 100.0f;
		_speed = max(20.0f, _speed);
	}

	// move direction control
	if (s_input.GetKey('W') == KEY_HOLD)
	{
		Vector v = _look * _speed * g_secondPerFrame;
		_pos += v;
	}
	if (s_input.GetKey('S') == KEY_HOLD)
	{
		Vector v = _look * -_speed * g_secondPerFrame;
		_pos += v;
	}
	if (s_input.GetKey('A') == KEY_HOLD)
	{
		Vector v = _right * -_speed * g_secondPerFrame;
		_pos = _pos + v;
	}
	if (s_input.GetKey('D') == KEY_HOLD)
	{
		Vector v = _right * _speed * g_secondPerFrame;
		_pos = _pos + v;
	}
	if (s_input.GetKey('Q') == KEY_HOLD)
	{
		Vector v = _up * -_speed * g_secondPerFrame;
		_pos = _pos + v;
	}
	if (s_input.GetKey('E') == KEY_HOLD)
	{
		Vector v = _up * _speed * g_secondPerFrame;
		_pos = _pos + v;
	}
	
	UpdateRotation(_roll, _pitch, _yaw);
	return true;
}

void Camera::UpdateVector()
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

	_right	= Vector::Normalise(_right);
	_up		= Vector::Normalise(_up);
	_look	= Vector::Normalise(_look);
}

void Camera::UpdateRotation(float roll, float pitch, float yaw)
{
	Quaternion rotQuat;
	Quaternion::RotationRollPitchYaw(&rotQuat, roll, pitch, yaw);

	Matrix rotMat;
	rotMat = Matrix::AffineTransformation(
		Vector(1.0f, 1.0f, 1.0f), nullptr, &rotQuat, &_pos);
	_viewMat = Matrix::Inverse(rotMat);

	UpdateVector();
	return;
}
