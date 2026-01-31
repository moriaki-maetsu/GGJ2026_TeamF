#pragma once

#include "../Objects/GameObjectManager.h"
#include "../Utility/Geometry.h"
#include "SceneType.h"

/// <summary>
/// シーン基底クラス（継承して使用してください）
/// </summary>
class SceneBase
{
protected:
	// オブジェクトマネージャー機能
	GameObjectManager* object_manager = nullptr;
	// カメラ座標
	Location2D camera_location = 0.0f;
	
public:
	SceneBase()
	{
		// オブジェクトマネージャーの情報を取得
		object_manager = GameObjectManager::Get();
	}
	virtual ~SceneBase()
	{
		// ゲームオブジェクトの解放忘れ防止
		SceneBase::Finalize();
	}

public:
	// 初期化
	virtual void Initialize() {}
	// 更新
	virtual eSceneType Update(float delta_second)
	{
		// ゲームオブジェクトの更新
		object_manager->Update(delta_second);

		// 現在のシーン情報を返却する
		return GetNowSceneType();
	}
	// 描画
	virtual void Draw() const
	{
		// ゲームオブジェクトの描画
		object_manager->Draw(camera_location);
	}
	// 終了時処理
	virtual void Finalize() 
	{
		// ゲームオブジェクトの解放
		object_manager->Release();
	}

public:
	// 現在のシーン情報を返す（オーバーライド必須）
	virtual eSceneType GetNowSceneType() const = 0;

};
