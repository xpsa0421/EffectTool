#pragma once
#include "GameCore.h"
#include "Camera.h"
#include "BaseObject.h"
#include "d3dx11Effect.h"

class Main : public GameCore
{
public:
	BaseObject	_floor;
public:
	bool	_renderFloor = true;
public:
	bool	Init()		override;
	bool	Frame()		override;
	bool	Render()	override;
	bool	Release()	override;
public:
	void	BuildEnvironment();
};

