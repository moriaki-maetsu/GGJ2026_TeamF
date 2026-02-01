#include "DxLib.h"
#include "TitleScene.h"
#include "../../Utility/InputManager.h"
#include"../../Utility/AssetContainer.h"

#define	D_WINDOW_SIZE_X	(1280)	// Windowサイズ（幅）
#define	D_WINDOW_SIZE_Y	(720)	// Windowサイズ（高さ）

#define START_BUTTON_UPPER_X 1280/2-100
#define START_BUTTON_LOWER_X 1280/2+100
#define START_BUTTON_UPPER_Y 720/4-20
#define START_BUTTON_LOWER_Y 720/2+20

#define END_BUTTON_UPPER_X 1280/2-100
#define END_BUTTON_LOWER_X 1280/2+100
#define END_BUTTON_UPPER_Y 720/2+40
#define END_BUTTON_LOWER_Y 720/2+80

int white = GetColor(255, 255, 255);
int black = GetColor(0, 0, 0);
int red = GetColor(255, 0, 0);
int startboxcolor, endboxcolor, bg_title;

void TitleScene::Initialize()
{
	AssetContainer* container = AssetContainer::Get();
	bg_title = container->GetImages("bg_title_01.png")[0];

	ChangeFont("HG創英角ｺﾞｼｯｸUB");
	SetFontSize(30);
	
}

eSceneType TitleScene::Update(float delta_second)
{
	InputManager* input =InputManager::Get();

	if (input->GetMouseLocation().x >= START_BUTTON_UPPER_X && input->GetMouseLocation().x <= START_BUTTON_LOWER_X)
	{
		if (input->GetMouseLocation().y >= START_BUTTON_UPPER_Y && input->GetMouseLocation().y <= START_BUTTON_LOWER_Y)
		{
			startboxcolor = red;

			if (input->GetMouseState(MOUSE_INPUT_LEFT) == eInputState::eClick)
			{
				
				return eSceneType::ePhaseOne;
			}
		}
		else
		{
			startboxcolor = white;
		}
	}
	else
	{
		startboxcolor = white;
	}

	if (input->GetMouseLocation().x >= END_BUTTON_UPPER_X && input->GetMouseLocation().x <= END_BUTTON_LOWER_X)
	{
		if (input->GetMouseLocation().y >= END_BUTTON_UPPER_Y && input->GetMouseLocation().y <= END_BUTTON_LOWER_Y)
		{
			endboxcolor = red;

			if (input->GetMouseState(MOUSE_INPUT_LEFT) == eInputState::eClick)
			{
				return eSceneType::eResult;
			}
		}
		else
		{
			endboxcolor = white;
		}
	}
	else
	{
		endboxcolor = white;
	}
	



	return GetNowSceneType();
}

void TitleScene::Draw() const
{
	DrawRotaGraph(1280 / 2, 720 / 2, 1.0, 0.0, bg_title, TRUE);
	DrawBox(START_BUTTON_UPPER_X, START_BUTTON_UPPER_Y, START_BUTTON_LOWER_X, START_BUTTON_LOWER_Y, startboxcolor, TRUE);
	DrawBox(END_BUTTON_UPPER_X, END_BUTTON_UPPER_Y, END_BUTTON_LOWER_X, END_BUTTON_LOWER_Y, endboxcolor, TRUE);
	DrawString(START_BUTTON_UPPER_X, START_BUTTON_UPPER_Y, "スタート", black);
	DrawString(END_BUTTON_UPPER_X, END_BUTTON_UPPER_Y, "エンド", black);
	
}

void TitleScene::Finalize()
{
}
