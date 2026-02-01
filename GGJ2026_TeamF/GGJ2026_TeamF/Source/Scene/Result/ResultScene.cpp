#include "ResultScene.h"
#include"DxLib.h"
#include"../../Utility/AssetContainer.h"
#include"../../Utility/InputManager.h"

#define UI_BUTTON_RETRY_X 1280-120
#define UI_BUTTON_RETRY_Y 720-150
#define UI_BUTTON_END_X 1280 -120
#define UI_BUTTON_END_Y  720 -80


std::vector<HeroData> data;

int character_set, total_hero, bg_result, button_retry_No, button_end_No;
int ui_button_retry[2], ui_button_end[2];

std::vector<int> ui_number;


void ResultScene::Initialize()
{
	AssetContainer* container = AssetContainer::Get();
	character_set = container->GetImages("character_set.png")[0];
	ui_number = container->GetImages("ui_number_01.png",11,11,1,374/11,68);
	bg_result= container->GetImages("bg_result_01.png")[0];
	ui_button_retry[0] = container->GetImages("ui_result_button_retry_01.png")[0];
	ui_button_retry[1] = container->GetImages("ui_result_button_retry_02.png")[0];
	ui_button_end[0] = container->GetImages("ui_result_button_end_01.png")[0];
	ui_button_end[1] = container->GetImages("ui_result_button_end_02.png")[0];
	data = Heros::Get()->GetHeros();
	total_hero = data.size();
	
	
}

eSceneType ResultScene::Update(float delta_second)
{
	InputManager* input = InputManager::Get();

	if (input->GetMouseLocation().x >= UI_BUTTON_RETRY_X-90 && input->GetMouseLocation().x <= UI_BUTTON_RETRY_X+90)
	{
		if (input->GetMouseLocation().y >= UI_BUTTON_RETRY_Y-15 && input->GetMouseLocation().y <= UI_BUTTON_RETRY_Y+15)
		{
			button_retry_No = 1;

			if (input->GetMouseState(MOUSE_INPUT_LEFT) == eInputState::eClick)
			{

				return eSceneType::ePhaseOne;
			}
		}
		else
		{
			button_retry_No = 0;

		}
	}
	else
	{
		button_retry_No = 0;
	}

	if (input->GetMouseLocation().x >= UI_BUTTON_END_X-90 && input->GetMouseLocation().x <= UI_BUTTON_END_X+90)
	{
		if (input->GetMouseLocation().y >= UI_BUTTON_END_Y-15 && input->GetMouseLocation().y <= UI_BUTTON_END_Y+15)
		{
			button_end_No = 1;

			if (input->GetMouseState(MOUSE_INPUT_LEFT) == eInputState::eClick)
			{
				return eSceneType::eTitle;
			}
		}
		else
		{
			button_end_No = 0;
		}
	}
	else
	{
		button_end_No = 0;
	}
	return GetNowSceneType();
}

void ResultScene::Draw() const
{

	DrawRotaGraph(1280 / 2, 720 / 2, 1.0, 0.0, bg_result, TRUE);
	DrawRotaGraph(UI_BUTTON_RETRY_X, UI_BUTTON_RETRY_Y, 1.3, 0.0, ui_button_retry[button_retry_No], TRUE);
	DrawRotaGraph(UI_BUTTON_END_X, UI_BUTTON_END_Y, 1.3, 0.0, ui_button_end[button_end_No], TRUE);

	
	
	int one = total_hero % 10;
	int ten = total_hero / 10 % 10;
	if (ten != 0)
	{
		DrawRotaGraph(900-50, 720 / 2+20, 1.0, 0.0, ui_number[ten], TRUE);
		DrawRotaGraph(900 , 720 / 2+20, 1.0, 0.0, ui_number[one], TRUE);
	}
	else
	{
		DrawRotaGraph(900 , 720 / 2+20, 1.0, 0.0, ui_number[one], TRUE);
	}
}

void ResultScene::Finalize()
{
}
