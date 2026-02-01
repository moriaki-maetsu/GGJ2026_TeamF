#include "ResultScene.h"
#include"DxLib.h"
#include"../../Utility/AssetContainer.h"
#include"../../Utility/InputManager.h"
#include"ResultData.h"

#define UI_BUTTON_RETRY_X 1280-120
#define UI_BUTTON_RETRY_Y 720-150
#define UI_BUTTON_END_X 1280 -120
#define UI_BUTTON_END_Y  720 -80


std::vector<HeroData> data;

int character_set, total_hero, bg_result, button_retry_No, button_title_No, bgm_result, se_button;
int ui_button_retry[2], ui_button_title[2];
float button_retry_angle, button_end_angle;
bool angleflg_retry, angleflg_end;
int enemy_count;

std::vector<int> ui_number;


void ResultScene::Initialize()
{
	AssetContainer* container = AssetContainer::Get();
	character_set = container->GetImages("character_set.png")[0];
	ui_number = container->GetImages("ui_number_02.png",11,11,1,374/11,68);
	bg_result= container->GetImages("bg_result_01.png")[0];
	ui_button_retry[0] = container->GetImages("ui_result_button_retry_01.png")[0];
	ui_button_retry[1] = container->GetImages("ui_result_button_retry_02.png")[0];
	ui_button_title[0] = container->GetImages("ui_result_button_end_01.png")[0];
	ui_button_title[1] = container->GetImages("ui_result_button_end_02.png")[0];
	bgm_result = container->GetSound("bgm_result.mp3");
	se_button = container->GetSound("se_result_button.mp3");
	ChangeVolumeSoundMem(255 * 50 / 100, bgm_result);
	PlaySoundMem(bgm_result, DX_PLAYTYPE_LOOP);
	data = Heros::Get()->GetHeros();
	enemy_count = ResultData::Get()->GetWrestlerCount();
	total_hero = data.size();
	button_retry_angle = 0.8f;
	button_end_angle = 0.6;
	angleflg_end = 0;
	angleflg_retry = 0;
	
		
}

eSceneType ResultScene::Update(float delta_second)
{
	InputManager* input = InputManager::Get();
	//float speed = 0.015f; // アニメーション速度（調整してください）

	//if (!angleflg_retry) { // 増やすモード
	//	button_retry_angle += speed * delta_second;
	//	if (button_retry_angle >= 0.9f) {
	//		button_retry_angle = 0.9f;
	//		angleflg_retry = true; // 減少モードへ
	//	}
	//}
	//else { // 減らすモード
	//	button_retry_angle -= speed * delta_second;
	//	if (button_retry_angle <= 0.8f) {
	//		button_retry_angle = 0.8f;
	//		angleflg_retry = false; // 増加モードへ
	//	}
	//}


	//if (!angleflg_end) {
	//	button_end_angle += speed * delta_second;
	//	if (button_end_angle >= 0.7f) {
	//		button_end_angle = 0.7f;
	//		angleflg_end = true;
	//	}
	//}
	//else {
	//	button_end_angle -= speed * delta_second;
	//	if (button_end_angle <= 0.6f) {
	//		button_end_angle = 0.6f;
	//		angleflg_end = false;
	//	}
	//}
	


	if (input->GetMouseLocation().x >= UI_BUTTON_RETRY_X-90 && input->GetMouseLocation().x <= UI_BUTTON_RETRY_X+90)
	{
		if (input->GetMouseLocation().y >= UI_BUTTON_RETRY_Y-15 && input->GetMouseLocation().y <= UI_BUTTON_RETRY_Y+15)
		{
	
			button_retry_No = 1;
			
			

			if (input->GetMouseState(MOUSE_INPUT_LEFT) == eInputState::eClick)
			{
				PlaySoundMem(se_button, DX_PLAYTYPE_BACK);
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
		
			button_title_No = 1;
			

			if (input->GetMouseState(MOUSE_INPUT_LEFT) == eInputState::eClick)
			{
				PlaySoundMem(se_button, DX_PLAYTYPE_BACK);
				return eSceneType::eTitle;
			}
		}
		else
		{
			button_title_No = 0;
		}
	}
	else
	{
		button_title_No = 0;
	}
	return GetNowSceneType();
}

void ResultScene::Draw() const
{

	int hero_one = total_hero % 10;
	int hero_ten = total_hero / 10 % 10;
	int enemy_one = enemy_count % 10;
	int enemy_ten = enemy_count / 10 % 10;

	DrawRotaGraph(1280 / 2, 720 / 2, 1.0, 0.0, bg_result, TRUE);
	DrawRotaGraph(UI_BUTTON_RETRY_X, UI_BUTTON_RETRY_Y, button_retry_angle, 0.0, ui_button_retry[button_retry_No], TRUE);
	DrawRotaGraph(UI_BUTTON_END_X, UI_BUTTON_END_Y, button_end_angle, 0.0, ui_button_title[button_title_No], TRUE);
	
	
	if (hero_ten != 0)
	{
		DrawRotaGraph(900-50, 720 / 2+20, 1.0, 0.0, ui_number[hero_ten], TRUE);
		DrawRotaGraph(900 , 720 / 2+20, 1.0, 0.0, ui_number[hero_one], TRUE);
	}
	else
	{
		DrawRotaGraph(900 , 720 / 2+20, 1.0, 0.0, ui_number[hero_one], TRUE);
	}
	if (enemy_ten != 0)
	{
		DrawRotaGraph(900 - 50, 720 / 2-50 , 1.0, 0.0, ui_number[enemy_ten], TRUE);
		DrawRotaGraph(900, 720 / 2-50 , 1.0, 0.0, ui_number[enemy_one], TRUE);
	}
	else
	{
		DrawRotaGraph(900, 720 / 2-50 , 1.0, 0.0, ui_number[enemy_one], TRUE);
	}
	


}

void ResultScene::Finalize()
{
	StopSoundMem(bgm_result);
}
