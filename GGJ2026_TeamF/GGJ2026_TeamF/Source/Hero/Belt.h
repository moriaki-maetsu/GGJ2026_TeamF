#pragma once
#include "../Utility/Geometry.h"
#include "Color.h"

#define BELT_SIZE_X	(50)
#define BELT_SIZE_Y	(50)

struct Belt
{
	Vector2D position = { 0.0f,0.0f };		// À•W
	eColor color = eColor::eNone;			// F						// —Í
	int image = -1;							// ‰æ‘œ
};