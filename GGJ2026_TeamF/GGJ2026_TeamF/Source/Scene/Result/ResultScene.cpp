#include "ResultScene.h"
#include"DxLib.h"
#include"../../Utility/AssetContainer.h"
#include"../../Utility/InputManager.h"

int hero_red, hero_blue, hero_green, hero_yellow,hero_pink;


void ResultScene::Initialize()
{
	AssetContainer* container = AssetContainer::Get();
	 hero_red = container->GetImages("character_red_01.png")[0];
	 hero_blue = container->GetImages("character_blue_01.png")[0];
	 hero_green = container->GetImages("character_green_01.png")[0];
	 hero_yellow = container->GetImages("character_yellow_01.png")[0];
	 hero_pink = container->GetImages("character_pink_01.png")[0];

}

eSceneType ResultScene::Update(float delta_second)
{

	return GetNowSceneType();
}

void ResultScene::Draw() const
{

	DrawRotaGraph(1280 / 2, 720 / 2, 0.5,0.0, hero_red, TRUE);
	DrawRotaGraph(1280 / 2+100, 720 / 2, 0.5,0.0, hero_blue, TRUE);
	DrawRotaGraph(1280 / 2-100, 720 / 2, 0.5,0.0, hero_green, TRUE);
	DrawRotaGraph(1280 / 2+250, 720 / 2, 0.5,0.0, hero_yellow, TRUE);
	DrawRotaGraph(1280 / 2-250, 720 / 2, 0.5,0.0, hero_pink, TRUE);


}

void ResultScene::Finalize()
{
}
