#pragma once
#include "../Utility/Geometry.h"
#include "Color.h"

#define HERO_SIZE_X	(45)
#define HERO_SIZE_Y	(60)



struct HeroData
{
	Vector2D position = { 0.0f,0.0f };		// À•W
	eColor color = eColor::eNone;			// F
	int power = 0;							// —Í
	int image = -1;							// ‰æ‘œ
};

