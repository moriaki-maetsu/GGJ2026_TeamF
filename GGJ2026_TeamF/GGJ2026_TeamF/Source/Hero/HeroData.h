#pragma once
#include "../Utility/Geometry.h"

#define HERO_SIZE_X	(75)
#define HERO_SIZE_Y	(100)

enum class eColor : unsigned char
{
	eNone,
	eRed,
	eBlue,
	eGreen,
	ePink
};

struct HeroData
{
	Vector2D position = { 0.0f,0.0f };		// À•W
	eColor color = eColor::eNone;			// F
	int power = 0;							// —Í
	int image = -1;							// ‰æ‘œ
};

