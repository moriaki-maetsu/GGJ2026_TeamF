#pragma once

#include "../Utility/Singleton.h"
#include "SceneBase.h"

// シーン管理クラス
class SceneManager final : public Singleton<SceneManager>
{
private:
	// 現在のシーン情報
	SceneBase* current_scene = nullptr;

public:
	SceneManager() = default;
	~SceneManager()
	{
		// 解放忘れ防止
		this->Finalize();
	}

public:
	// 初期化
	void Initialize(eSceneType new_scene_type = eSceneType::eTitle);
	// 更新
	bool Update(float delta_second);
	// 終了時
	void Finalize();

private:
	// シーン切り替え
	void ChangeScene(eSceneType new_scene_type);

};
