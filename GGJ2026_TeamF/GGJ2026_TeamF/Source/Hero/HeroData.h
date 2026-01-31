#pragma once
#include "../Utility/Geometry.h"

#define HERO_SIZE_X	(75)
#define HERO_SIZE_Y	(100)

enum class eColor : unsigned char
{
	eRed,
	eBlue,
	eGreen,
	ePink
};

struct HeroData
{
	Vector2D position;		// À•W
	eColor color;			// F
	int power;				// —Í
	int image;				// ‰æ‘œ
};

