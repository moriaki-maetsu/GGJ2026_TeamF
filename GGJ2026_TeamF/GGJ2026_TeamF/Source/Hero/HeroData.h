#pragma once
#include "../Utility/Geometry.h"
#include "Color.h"
#include "DxLib.h"

#define HERO_SIZE_X	(50)
#define HERO_SIZE_Y	(65)

struct HeroData
{
	Vector2D position = { 0.0f,0.0f };		// À•W
	eColor color = eColor::eNone;			// F
	int power = 0;							// —Í
	int image = -1;							// ‰æ‘œ
	bool change_flag = FALSE;
};

