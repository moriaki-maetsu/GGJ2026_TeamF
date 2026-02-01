#pragma once
#include "../Utility/Geometry.h"
#include "Color.h"
#include "DxLib.h"

#define BELT_SIZE_X	(50)
#define BELT_SIZE_Y	(50)

struct Belt
{
	Vector2D position = { 0.0f,0.0f };		// ç¿ïW
	eColor color = eColor::eNone;			// êF
	int image = -1;							// âÊëú
	bool drag_flag = FALSE;
};