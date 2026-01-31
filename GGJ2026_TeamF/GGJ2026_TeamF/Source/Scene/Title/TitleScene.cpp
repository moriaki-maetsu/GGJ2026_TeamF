#include "DxLib.h"
#include "TitleScene.h"
#include "../../Utility/InputManager.h"

void TitleScene::Initialize()
{
}

eSceneType TitleScene::Update(float delta_second)
{
	InputManager* input =InputManager::Get();
	if (input->GetMouseState(MOUSE_INPUT_LEFT) == eInputState::eClick)
	{
		return eSceneType::ePhaseOne;
	}
	return GetNowSceneType();
}

void TitleScene::Draw() const
{
	DrawString(0, 0, "タイトル画面", GetColor(255, 255, 255));
}

void TitleScene::Finalize()
{
}
