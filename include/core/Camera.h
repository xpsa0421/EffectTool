#pragma once
#include "Input.h"
#include "Frustum.h"

class Camera 
{
protected:
	float		_speed = 10.0f;
public:
	Frustum		_frustum;
public:
	Matrix		_viewMat;
	Matrix		_projMat;
	Vector		_pos;
	Vector		_target;
	Vector		_up;
	Vector		_right;
	Vector		_look;
public:
	float		_near;
	float		_far;
	float		_fov;
	float		_aspect;
	float		_yaw;
	float		_pitch;
	float		_roll;
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual void	UpdateVector();
	virtual void	UpdateRotation(float roll, float pitch, float yaw);
	virtual void	CreateViewMatrix(Vector pos, Vector target, Vector up);
	virtual void	CreateProjMatrix(float n, float f, float fov, float aspect);
	virtual void	UpdateProjMatrix(float aspect);
public:
	Camera();
};

