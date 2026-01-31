#include "DxLib.h"
#include "PhaseOne.h"
#include "../../Utility/AssetContainer.h"

void PhaseOne::Initialize()
{
	AssetContainer* container = AssetContainer::Get();
	hero[0].color = eColor::eRed;
	hero[0].image = container->GetImages("Hero_Red.png")[0];
	hero[0].position.x = 0.0f;
	hero[0].position.y = 0.0f;
	hero[0].power = 0;

	hero[1].color = eColor::eRed;
	hero[1].image = container->GetImages("Hero_Blue.png")[0];
	hero[1].position.x = 0.0f;
	hero[1].position.y = 0.0f;
	hero[1].power = 0;

	display_count = 0;
}

eSceneType PhaseOne::Update(float delta_second)
{
	if (++display_count >= 120)
	{
		for (int i = 0; i < sizeof(hero) / sizeof(hero[0]); i++)
		{
			if (hero[i].power == 0)
			{
				hero[i].position.x = 100.0f;
				hero[i].position.y = 300.0f;
				hero[i].power = 3;
				break;
			}
		}
		display_count = 0;
	}

	return GetNowSceneType();
}

void PhaseOne::Draw() const
{
	for (int i = 0; i < sizeof(hero) / sizeof(hero[0]); i++)
	{
		if (hero[i].power != 0)
		{
			DrawRotaGraph(hero[i].position.x, hero[i].position.y, 1.0, 0.0, hero[i].image, TRUE);
		}
	}
}

void PhaseOne::Finalize()
{
}
