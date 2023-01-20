#pragma once
#include "GameCore.h"

class Main : public GameCore
{
public:
	bool	Init()		override;
	bool	Frame()		override;
	bool	Render()	override;
	bool	Release()	override;
};

