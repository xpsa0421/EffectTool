#pragma once
#include "GameCore.h"
#include "Camera.h"
#include "Object.h"
#include "d3dx11Effect.h"

class Main : public GameCore
{
public:
	bool	Init()		override;
	bool	Frame()		override;
	bool	Render()	override;
	bool	Release()	override;
public:
	void	BuildEnvironment();
};

