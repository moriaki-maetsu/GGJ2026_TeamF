#include "PhaseTwo.h"
#include "../../Utility/AssetContainer.h"
#include <DxLib.h>

void PhaseTwo::Initialize()
{
	int handle = AssetContainer::Get()->GetImages("Assets/Images/ProtoHero.png")[0];
	for (int i = 0; i < 10; i++)
	{
		heros.push_back({ Vector2D{100.f * 30.f * i,10},eColor::eRed,10,handle });
	}
}

eSceneType PhaseTwo::Update(float delta_second)
{
	return GetNowSceneType();
}

void PhaseTwo::Draw() const
{
	for (const HeroData& hero : heros)
	{
		DrawRotaGraphF(hero.position.x, hero.position.y, 1.0f, 0.0f, hero.image, TRUE);
	}
}

void PhaseTwo::Finalize()
{
}
