#pragma once
#include "../Utility/Geometry.h"

#define HERO_SIZE_X	(10)
#define HERO_SIZE_Y	(10)

enum class eColor : unsigned char
{
	eRed,
	eBlue,
};

struct HeroData
{
	Vector2D position;		// À•W
	eColor color;			// F
	int power;				// —Í
	int image;				// ‰æ‘œ
};

