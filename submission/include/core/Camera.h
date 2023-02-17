#pragma once
#include "Input.h"

class Camera 
{
protected:
	bool		speed_controllable_;
	float		move_speed_;
	float		nearZ_;
	float		farZ_;
	float		fov_;
	float		aspect_ratio_;

	float		yaw_;
	float		pitch_;
	float		roll_;

public:
	XMFLOAT4X4	view_;
	XMFLOAT4X4	proj_;

	XMFLOAT3	pos_;
	XMFLOAT3	target_;
	XMFLOAT3	up_;
	XMFLOAT3	right_;
	XMFLOAT3	look_;
	
public:
	virtual void	Init();

	void SetSpeedControllable(bool speed_controllable);
	void SetPosition(float x, float y, float z);
	void SetTarget(float x, float y, float z);
	void SetWorldUp(float x, float y, float z);
		 
	void Move(XMFLOAT3 direction, float distance);

	virtual bool	Frame();
	virtual void	UpdateVector();
	virtual void	UpdateRotation(float roll, float pitch, float yaw);
	virtual void	SetView(XMFLOAT3 pos = XMFLOAT3(0, 0, -1), XMFLOAT3 target = XMFLOAT3(0, 0, 0), XMFLOAT3 up = XMFLOAT3(0, 1, 0));
	virtual void	SetLens(float nearZ, float farZ, float fov, float aspect_ratio);
};

