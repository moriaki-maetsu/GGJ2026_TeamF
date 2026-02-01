#include "DxLib.h"
#include "PhaseOne.h"
#include "../../Utility/AssetContainer.h"
#include "../../Utility/InputManager.h"
#include "../../Hero/Heros.h"
#include "stdlib.h"

#define RED_BELT_X (400.0f)
#define BLUE_BELT_X (520.0f)
#define GREEN_BELT_X (640.0f)
#define PINK_BELT_X (760.0f)
#define YELLOW_BELT_X (880.0f)

void PhaseOne::Initialize()
{
	AssetContainer* container = AssetContainer::Get();
	hero[0].color = eColor::eRed;
	hero[0].image = container->GetImages("character_red_01.png")[0];
	hero[0].position.x = 0.0f;
	hero[0].position.y = 0.0f;
	hero[0].power = 0;

	hero[1].color = eColor::eBlue;
	hero[1].image = container->GetImages("character_blue_01.png")[0];
	hero[1].position.x = 0.0f;
	hero[1].position.y = 0.0f;
	hero[1].power = 0;

	hero[2].color = eColor::eGreen;
	hero[2].image = container->GetImages("character_green_01.png")[0];
	hero[2].position.x = 0.0f;
	hero[2].position.y = 0.0f;
	hero[2].power = 0;

	hero[3].color = eColor::ePink;
	hero[3].image = container->GetImages("character_pink_01.png")[0];
	hero[3].position.x = 0.0f;
	hero[3].position.y = 0.0f;
	hero[3].power = 0;

	hero[4].color = eColor::eYellow;
	hero[4].image = container->GetImages("character_yellow_01.png")[0];
	hero[4].position.x = 0.0f;
	hero[4].position.y = 0.0f;
	hero[4].power = 0;

	belt[0].color = eColor::eRed;
	belt[0].image = container->GetImages("icon_belt_red_02.png")[0];
	belt[0].position.x = RED_BELT_X;
	belt[0].position.y = 600.0f;

	belt[1].color = eColor::eBlue;
	belt[1].image = container->GetImages("icon_belt_blue_02.png")[0];
	belt[1].position.x = BLUE_BELT_X;
	belt[1].position.y = 600.0f;

	belt[2].color = eColor::eGreen;
	belt[2].image = container->GetImages("icon_belt_green_02.png")[0];
	belt[2].position.x = GREEN_BELT_X;
	belt[2].position.y = 600.0f;

	belt[3].color = eColor::ePink;
	belt[3].image = container->GetImages("icon_belt_pink_02.png")[0];
	belt[3].position.x = PINK_BELT_X;
	belt[3].position.y = 600.0f;

	belt[4].color = eColor::eYellow;
	belt[4].image = container->GetImages("icon_belt_yellow_02.png")[0];
	belt[4].position.x = YELLOW_BELT_X;
	belt[4].position.y = 600.0f;

	display_count = 0;
}

eSceneType PhaseOne::Update(float delta_second)
{
	//ヒーロー生成処理
	if (++display_count >= 120)
	{
		do
		{
			bool exit = FALSE;
			int form_num = rand() % 5;

			for (int i = 0; i < sizeof(hero) / sizeof(hero[0]); i++)
			{
				if (i == form_num)
				{
					if (hero[form_num].power == 0)
					{
						hero[i].position.x = 100.0f;
						hero[i].position.y = 200.0f;
						hero[i].power = rand() % 5 + 1;
						exit = TRUE;
					}
				}
			}

			if (exit)
			{
				break;
			}
		} while (TRUE);
		
		
		display_count = 0;
	}

	//ヒーロー移動処理
	for (int i = 0; i < sizeof(hero) / sizeof(hero[0]); i++)
	{
		if (hero[i].power != 0)
		{
			hero[i].position.x += 3.0f;
			if (hero[i].position.x >= 1280)
			{
				hero[i].position.x = 0.0f;
				hero[i].position.y = 0.0f;
				hero[i].power = 0;
			}
		}
	}

	InputManager* input = InputManager::Get();
	
	//ベルトドラッグ処理
	if (input->GetMouseState(MOUSE_INPUT_LEFT) == eInputState::eClick)
	{
		for (int i = 0; i < sizeof(belt) / sizeof(belt[0]); i++)
		{
			Vector2D collision_LeftUpper = { belt[i].position.x - BELT_SIZE_X , belt[i].position.y - BELT_SIZE_Y };
			Vector2D collision_RightLower = { belt[i].position.x + BELT_SIZE_X , belt[i].position.y + BELT_SIZE_Y };
			if (input->GetMouseLocation().x >= collision_LeftUpper.x && input->GetMouseLocation().x <= collision_RightLower.x)
			{
				if (input->GetMouseLocation().y >= collision_LeftUpper.y && input->GetMouseLocation().y <= collision_RightLower.y)
				{
					belt[i].drag_flag = TRUE;
					break;
				}
			}
		}
	}
	else if (input->GetMouseState(MOUSE_INPUT_LEFT) == eInputState::ePressed)
	{
		for (int i = 0; i < sizeof(belt) / sizeof(belt[0]); i++)
		{
			if (belt[i].drag_flag)
			{
				belt[i].position.x = input->GetMouseLocation().x;
				belt[i].position.y = input->GetMouseLocation().y;
				break;
			}
		}
	}
	else if (input->GetMouseState(MOUSE_INPUT_LEFT) == eInputState::eRelease)
	{
		for (int i = 0; i < sizeof(belt) / sizeof(belt[0]); i++)
		{
			if (belt[i].drag_flag)
			{
				switch (belt[i].color)
				{
				case eColor::eRed:
					belt[i].position.x = RED_BELT_X;
					break;
				case eColor::eBlue:
					belt[i].position.x = BLUE_BELT_X;
					break;
				case eColor::eGreen:
					belt[i].position.x = GREEN_BELT_X;
					break;
				case eColor::ePink:
					belt[i].position.x = PINK_BELT_X;
					break;
				case eColor::eYellow:
					belt[i].position.x = YELLOW_BELT_X;
					break;
				}
				belt[i].position.y = 600.0f;
				belt[i].drag_flag = FALSE;
				break;
			}
		}
	}

	//ヒーロー当たり判定処理
	
	Heros* heros = Heros::Get();
	AssetContainer* container = AssetContainer::Get();
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
						switch (hero[i].color)
						{
						case eColor::eRed:
							hero[i].image = container->GetImages("character_red_02.png")[0];
							break;
						case eColor::eBlue:
							hero[i].image = container->GetImages("character_blue_02.png")[0];
							break;
						case eColor::eGreen:
							hero[i].image = container->GetImages("character_green_02.png")[0];
							break;
						case eColor::ePink:
							hero[i].image = container->GetImages("character_pink_02.png")[0];
							break;
						case eColor::eYellow:
							hero[i].image = container->GetImages("character_yellow_02.png")[0];
							break;
						}
						heros->SetHeros(hero[i]);
						hero[i].position.x = 0.0f;
						hero[i].position.y = 0.0f;
						hero[i].power = 0;
						break;
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

	for (int i = 0; i < sizeof(belt) / sizeof(belt[0]); i++)
	{
		DrawRotaGraph(belt[i].position.x, belt[i].position.y, 1.0, 0.0, belt[i].image, TRUE);
		Vector2D collision_LeftUpper = { belt[i].position.x - BELT_SIZE_X , belt[i].position.y - BELT_SIZE_Y };
		Vector2D collision_RightLower = { belt[i].position.x + BELT_SIZE_X , belt[i].position.y + BELT_SIZE_Y };
		DrawBox(collision_LeftUpper.x, collision_LeftUpper.y, collision_RightLower.x, collision_RightLower.y, GetColor(255, 255, 255), FALSE);
	}

	Heros* heros = Heros::Get();
	for (int i = 0; i < heros->GetHeros().size(); i++)
	{
		DrawFormatString(0, 0 + (i * 20), GetColor(255, 255, 255), "%d", heros->GetHeros().at(i).power);
	}
}

void PhaseOne::Finalize()
{
}
