#include "Camera.h"

void Camera::Init()
{
	speed_controllable_		= false;
	move_speed_				= 10.0f;
	nearZ_					= 0.0f;
	roll_ = pitch_ = yaw_	= 0.0f;
	pos_					= XMFLOAT3(0, 0, -1);
	target_					= XMFLOAT3(0, 0, 0);
	up_						= XMFLOAT3(0, 1, 0);
	right_					= XMFLOAT3(1, 0, 0);
}

void Camera::SetSpeedControllable(bool speed_controllable)
{
	speed_controllable_ = speed_controllable;
}

void Camera::SetPosition(float x, float y, float z)
{
	pos_ = XMFLOAT3(x, y, z);
}

void Camera::SetTarget(float x, float y, float z)
{
	target_ = XMFLOAT3(x, y, z);
}

void Camera::SetWorldUp(float x, float y, float z)
{
	up_ = XMFLOAT3(x, y, z);
}

void Camera::Move(XMFLOAT3 direction, float distance)
{
	XMVECTOR dist_v = XMVectorReplicate(distance);
	XMVECTOR dir_v = XMLoadFloat3(&direction);
	XMVECTOR pos_v = XMLoadFloat3(&pos_);
	XMStoreFloat3(&pos_, XMVectorMultiplyAdd(dist_v, dir_v, pos_v));
}

void Camera::SetView(XMFLOAT3 pos, XMFLOAT3 target, XMFLOAT3 up)
{
	pos_ = pos;
	target_ = target;
	up_ = up;

	XMMATRIX view_m = XMMatrixLookAtLH(XMLoadFloat3(&pos_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
	XMStoreFloat4x4(&view_, view_m);

	XMMATRIX view_inv_m = XMMatrixInverse(nullptr, view_m);
	XMFLOAT4X4 view_inv;
	XMStoreFloat4x4(&view_inv, view_inv_m);
	float fLen = sqrtf(view_inv._33 * view_inv._33 + view_inv._31 * view_inv._31);

	yaw_ = atan2f(view_inv._31, view_inv._33);
	pitch_ = -atan2f(view_inv._32, fLen);
}

void Camera::SetLens(float nearZ, float farZ, float fov, float aspect_ratio)
{
	nearZ_ = nearZ;
	farZ_ = farZ;
	fov_ = fov;
	aspect_ratio_ = aspect_ratio;
	
	XMMATRIX proj_m = XMMatrixPerspectiveFovLH(fov, aspect_ratio, nearZ, farZ);
	XMStoreFloat4x4(&proj_, proj_m);
}

bool Camera::Frame()
{
	// view direction control
	if (s_input.GetKey(VK_MBUTTON) == KEY_HOLD)
	{
		yaw_ += s_input.mouse_offset_.x * 0.002f;
		pitch_ += s_input.mouse_offset_.y * 0.002f;
	}
	UpdateRotation(roll_, pitch_, yaw_);

	// movement speed control
	if (speed_controllable_)
	{
		if (s_input.GetKey(VK_SPACE) == KEY_HOLD)
		{
			move_speed_ += g_delta_time * 100.0f;
			move_speed_ = min(100.0f, move_speed_);
		}
		else
		{
			move_speed_ -= g_delta_time * 100.0f;
			move_speed_ = max(10.0f, move_speed_);
		}
	}

	// movement control
	if (s_input.GetKey('W') == KEY_HOLD)
	{
		Move(look_, move_speed_ * g_delta_time);
	}
	if (s_input.GetKey('S') == KEY_HOLD)
	{
		Move(look_, -move_speed_ * g_delta_time);
	}
	if (s_input.GetKey('A') == KEY_HOLD)
	{
		Move(right_, -move_speed_ * g_delta_time);
	}
	if (s_input.GetKey('D') == KEY_HOLD)
	{
		Move(right_, move_speed_ * g_delta_time);
	}
	if (s_input.GetKey('Q') == KEY_HOLD)
	{
		Move(up_, -move_speed_ * g_delta_time);
	}
	if (s_input.GetKey('E') == KEY_HOLD)
	{
		Move(up_, move_speed_ * g_delta_time);
	}
	UpdateVector();
	return true;
}

void Camera::UpdateVector()
{
	right_.x = view_._11;
	right_.y = view_._21;
	right_.z = view_._31;

	up_.x = view_._12;
	up_.y = view_._22;
	up_.z = view_._32;

	look_.x = view_._13;
	look_.y = view_._23;
	look_.z = view_._33;

	XMVECTOR right_v = XMVector3Normalize(XMLoadFloat3(&right_));
	XMVECTOR up_v = XMVector3Normalize(XMLoadFloat3(&up_));
	XMVECTOR look_v = XMVector3Normalize(XMLoadFloat3(&look_));

	XMStoreFloat3(&right_, right_v);
	XMStoreFloat3(&up_, up_v);
	XMStoreFloat3(&look_, look_v);
}

void Camera::UpdateRotation(float roll, float pitch, float yaw)
{
	XMVECTOR scaling = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	XMVECTOR rotation_center = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR rotation_q = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
	XMVECTOR translation = XMLoadFloat3(&pos_);

	XMMATRIX rotation_m = XMMatrixAffineTransformation(scaling, rotation_center, rotation_q, translation);
	XMStoreFloat4x4(&view_, XMMatrixInverse(nullptr, rotation_m));

	UpdateVector();
	return;
}
