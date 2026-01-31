#pragma once

#include "../Utility/Geometry.h"
#include "../Utility/Collision.h"

#include "../Utility/AssetContainer.h"
#include "../Utility/InputManager.h"
#include "GameObjectManager.h"

#include <cstdint>

/// <summary>
/// ゲームオブジェクト基底クラス（画面上に配置するオブジェクト基底クラス）
/// </summary>
class GameObject
{
protected:
	InputManager* input = nullptr;			// 入力情報
	AssetContainer* container = nullptr;	// アセット情報
	GameObjectManager* game_object_manager = nullptr;	// ゲームオブジェクト管理

protected:
	Location2D location = 0.0f;				// 位置情報
	Collision collision;					// 当たり判定情報
	uint8_t z_layer = 1U;					// Zレイヤー（数字が大きいほど前に描画される）
	bool is_mobility = false;				// 可動性

public:
	GameObject()
	{
		input = InputManager::Get();
		container = AssetContainer::Get();
		game_object_manager = GameObjectManager::Get();
	}
	virtual ~GameObject() = default;

public:
	// 初期化処理
	virtual void Initialize() {}
	// 更新処理（毎フレーム呼ばれる）
	virtual void Update(float delta_second) {}
	// 更新処理（1/60sに1回呼ばれる）
	virtual void FixedUpdate() {}
	// 描画処理
	virtual void Draw(const Location2D& screen_offset) const {}
	// 終了時処理
	virtual void Finalize() {}

public:
	// 当たり判定通知処理
	virtual void OnHitCollision(GameObject* hit_object) {}

public:
	// 位置情報の取得
	const Location2D& GetLocation() const
	{
		return location;
	}
	// コリジョン情報の取得
	const Collision& GetCollision() const
	{
		return collision;
	}
	// レイヤー情報の取得
	const uint8_t GetZLayer() const
	{
		return z_layer;
	}
	// 可動性情報の取得
	const bool IsMobility() const
	{
		return is_mobility;
	}
	// 位置情報の設定
	void SetLocation(const Location2D& location)
	{
		this->location = location;
	}

protected:
	// 新しいゲームオブジェクトを生成する
	template <class OBJECT, typename... Args>
	OBJECT* CreateGameObject(const Location2D& new_location, Args&& ... args)
	{
		return game_object_manager->CreateGameObject<OBJECT>(new_location, std::forward<Args>(args)...);
	}
};
