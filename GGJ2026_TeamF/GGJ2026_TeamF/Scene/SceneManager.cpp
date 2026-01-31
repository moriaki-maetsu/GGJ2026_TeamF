#include "SceneManager.h"
#include "SceneBase.h"
#include "SceneFactory.h"
#include "DxLib.h"

void SceneManager::Initialize(eSceneType new_scene_type)
{
	// 引数で渡された情報を基にシーンを生成する
	ChangeScene(new_scene_type);
}

bool SceneManager::Update(float delta_second)
{
	// 現在シーンの更新
	eSceneType next_scene_type = current_scene->Update(delta_second);

	// 現在シーンの描画処理
	ClearDrawScreen();
	current_scene->Draw();
	ScreenFlip();

	// 現在と次のシーン状態が違っていたら、切り替えを行う
	if (next_scene_type != current_scene->GetNowSceneType())
	{
		// 次のシーンタイプが無い場合、処理を終了する
		if (next_scene_type == eSceneType::eExit)
		{
			return false;
		}
		// シーン切り替え処理
		ChangeScene(next_scene_type);
	}

	return true;
}

void SceneManager::Finalize()
{
	// シーン情報が残っていたら、削除する
	if (current_scene)
	{
		current_scene->Finalize();
		delete current_scene;
		current_scene = nullptr;
	}
}

void SceneManager::ChangeScene(eSceneType new_scene_type)
{
	// 現在のシーンが存在していたら、削除する
	if (current_scene)
	{
		current_scene->Finalize();
		delete current_scene;
	}

	// 新しいシーンを生成と初期化を行う
	current_scene = SceneFactory::CreateScene(new_scene_type);
	if (current_scene == nullptr)
	{
		// シーン生成に失敗したら、警告を投げる
		throw std::runtime_error("シーン生成ができませんでした");
	}
	current_scene->Initialize();
}
