#include "DxLib.h"
#include "TitleScene.h"
#include "../../Utility/InputManager.h"
#include"../../Utility/AssetContainer.h"

#define	D_WINDOW_SIZE_X	(1280)	// Windowサイズ（幅）
#define	D_WINDOW_SIZE_Y	(720)	// Windowサイズ（高さ）

#define START_BUTTON_X 1280/2-110
#define START_BUTTON_Y 720/4*3


#define END_BUTTON_X 1280/2+110
#define END_BUTTON_Y 720/4*3


int white = GetColor(255, 255, 255);
int black = GetColor(0, 0, 0);
int red = GetColor(255, 0, 0);
int startboxcolor, endboxcolor, bg_title, bgm_title, se_title_button, voice_start, button_start_No, button_end_No;
int ui_button_start[2], ui_button_end[2];

void TitleScene::Initialize()
{
	AssetContainer* container = AssetContainer::Get();
	bg_title = container->GetImages("bg_title_01.png")[0];
	ui_button_start[0] = container->GetImages("ui_title_start_01.png")[0];
	ui_button_start[1] = container->GetImages("ui_title_start_02.png")[0];
	ui_button_end[0] = container->GetImages("ui_title_end_01.png")[0];
	ui_button_end[1] = container->GetImages("ui_title_end_02.png")[0];

	bgm_title = container->GetSound("bgm_title.mp3");
	se_title_button = container->GetSound("se_title_button.mp3");
	voice_start = container->GetSound("voice_title_start.mp3");
	ChangeVolumeSoundMem(255 * 30 / 100, bgm_title);
	ChangeVolumeSoundMem(255 * 50 / 100, se_title_button);
	ChangeVolumeSoundMem(255 * 60 / 100, voice_start);
	PlaySoundMem(bgm_title, DX_PLAYTYPE_LOOP);
	
}

eSceneType TitleScene::Update(float delta_second)
{
	InputManager* input =InputManager::Get();

	if (input->GetMouseLocation().x >= START_BUTTON_X-100 && input->GetMouseLocation().x <= START_BUTTON_X+100)
	{
		if (input->GetMouseLocation().y >= START_BUTTON_Y-45 && input->GetMouseLocation().y <= START_BUTTON_Y+45)
		{
		
			button_start_No = 1;

			if (input->GetMouseState(MOUSE_INPUT_LEFT) == eInputState::eClick)
			{
				PlaySoundMem(se_title_button, DX_PLAYTYPE_BACK);
				PlaySoundMem(voice_start, DX_PLAYTYPE_NORMAL);
				return eSceneType::ePhaseOne;
			}
		}
		else
		{
			button_start_No = 0;
		}
	}
	else
	{
		button_start_No = 0;
	}

	if (input->GetMouseLocation().x >= END_BUTTON_X-100 && input->GetMouseLocation().x <= END_BUTTON_X+100)
	{
		if (input->GetMouseLocation().y >= END_BUTTON_Y-45 && input->GetMouseLocation().y <= END_BUTTON_Y+45)
		{
			button_end_No = 1;

			if (input->GetMouseState(MOUSE_INPUT_LEFT) == eInputState::eClick)
			{
				PlaySoundMem(se_title_button, DX_PLAYTYPE_BACK);
				return eSceneType::eExit;
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

void TitleScene::Draw() const
{
	DrawRotaGraph(1280 / 2, 720 / 2, 1.0, 0.0, bg_title, TRUE);
	DrawRotaGraph(START_BUTTON_X, START_BUTTON_Y, 1.0, 0.0, ui_button_start[button_start_No], TRUE);
	DrawRotaGraph(END_BUTTON_X, END_BUTTON_Y, 1.0, 0.0, ui_button_end[button_end_No], TRUE);

	
	
}

void TitleScene::Finalize()
{
	StopSoundMem(bgm_title);
}
