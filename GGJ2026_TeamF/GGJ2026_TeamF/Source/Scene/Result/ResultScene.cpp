#include "ResultScene.h"
#include"DxLib.h"
#include"../../Utility/AssetContainer.h"
#include"../../Utility/InputManager.h"

std::vector<HeroData> data;

int character_set, total_hero, bg_result;


std::vector<int> ui_number;


void ResultScene::Initialize()
{
	AssetContainer* container = AssetContainer::Get();
	character_set = container->GetImages("character_set.png")[0];
	ui_number = container->GetImages("ui_number_01.png",10,10,1,50,100);
	bg_result= container->GetImages("bg_result_01.png")[0];
	data = Heros::Get()->GetHeros();
	total_hero = 5;//data.size();
	
}

eSceneType ResultScene::Update(float delta_second)
{

	return GetNowSceneType();
}

void ResultScene::Draw() const
{

	DrawRotaGraph(1280 / 2, 720 / 2, 1.0, 0.0, bg_result, TRUE);
	DrawRotaGraph(1280 / 2, 720 / 2+200, 0.5,0.0, character_set, TRUE);
	
	
	int one = total_hero % 10;
	int ten = total_hero / 10 % 10;
	if (ten != 0)
	{
		DrawRotaGraph(1280 / 2, 720 / 2, 1.5, 0.0, ui_number[ten], TRUE);
		DrawRotaGraph(1280 / 2 + 50, 720 / 2, 1.5, 0.0, ui_number[one], TRUE);
	}
	else
	{
		DrawRotaGraph(1280 / 2 , 720 / 2, 1.5, 0.0, ui_number[one], TRUE);
	}
}

void ResultScene::Finalize()
{
}
