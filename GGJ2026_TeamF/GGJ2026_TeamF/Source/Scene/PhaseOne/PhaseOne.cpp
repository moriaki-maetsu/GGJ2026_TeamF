#include "DxLib.h"
#include "PhaseOne.h"
#include "../../Utility/AssetContainer.h"
#include "../../Utility/InputManager.h"

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
	//ヒーロー生成処理
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

	//ヒーロー移動処理
	for (int i = 0; i < sizeof(hero) / sizeof(hero[0]); i++)
	{
		if (hero[i].power != 0)
		{
			hero[i].position.x += 1.0f;
		}
	}

	//ヒーロー当たり判定処理
	InputManager* input = InputManager::Get();
	if (input->GetMouseState(MOUSE_INPUT_LEFT) == eInputState::eClick)
	{
		for (int i = 0; i < sizeof(hero) / sizeof(hero[0]); i++)
		{
			if (hero[i].power != 0)
			{
				Vector2D collision_LeftUpper = { hero[i].position.x - HERO_SIZE_X , hero[i].position.y - HERO_SIZE_Y };
				Vector2D collision_RightLower = { hero[i].position.x + HERO_SIZE_X , hero[i].position.y + HERO_SIZE_Y };
				if (input->GetMouseLocation().x >= collision_LeftUpper.x && input->GetMouseLocation().x <= collision_RightLower.x)
				{
					if (input->GetMouseLocation().y >= collision_LeftUpper.y && input->GetMouseLocation().y <= collision_RightLower.y)
					{
						hero[i].position.x = 0.0f;
						hero[i].position.y = 0.0f;
						hero[i].power = 0;
					}
				}
			}
		}
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
			Vector2D collision_LeftUpper = { hero[i].position.x - HERO_SIZE_X , hero[i].position.y - HERO_SIZE_Y };
			Vector2D collision_RightLower = { hero[i].position.x + HERO_SIZE_X , hero[i].position.y + HERO_SIZE_Y };
			DrawBox(collision_LeftUpper.x, collision_LeftUpper.y, collision_RightLower.x, collision_RightLower.y, GetColor(255, 255, 255), FALSE);
		}
	}
}

void PhaseOne::Finalize()
{
}
