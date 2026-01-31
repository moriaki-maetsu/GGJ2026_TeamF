#include "GameObjectManager.h"
#include "GameObject.h"

void GameObjectManager::Update(float delta_second)
{
	// 1フレームの経過時間を加算する
	fixed_time += delta_second;
	bool fixed_flag = (fixed_time >= D_FIXED_TIME);
	if (fixed_flag)
	{
		fixed_time -= D_FIXED_TIME;
	}

	// 生成すべきゲームオブジェクトがあるか確認する
	CheckCreateGameObject();

	// ゲームオブジェクトの更新
	for (GameObject* object : game_object_list)
	{
		// フレームレート依存による更新
		object->Update(delta_second);

		// 固定フレームレートによる更新
		if (fixed_flag)
		{
			object->FixedUpdate();

			// 動くオブジェクトであれば、当たり判定の確認をする
			if (object->IsMobility())
			{
				const size_t list_size = game_object_list.size();
				for (size_t index = 0ULL; index < list_size; index++)
				{
					CheckHitCollision(object, game_object_list[index]);
				}
			}
		}
	}

	// 破棄すべきゲームオブジェクトがあるか確認する
	CheckDestroyGameObject();
}

void GameObjectManager::Draw(const Location2D& screen_offset) const
{
	// ゲームオブジェクトの描画
	for (GameObject* object : game_object_list)
	{
		object->Draw(screen_offset);
	}
}

void GameObjectManager::DestroyGameObject(GameObject* target)
{
	// ゲームオブジェクトが存在していて、
	if (target)
	{
		// 破棄すべきオブジェクトリスト内に存在しないなら
		for (GameObject* obj : destroy_list)
		{
			if (obj == target)
			{
				return;
			}
		}
		// 破棄すべきオブジェクトリストに追加する
		destroy_list.emplace_back(target);
	}
}

void GameObjectManager::Release()
{
	// 生成するオブジェクトがあるか確認する
	CheckCreateGameObject();

	while (not game_object_list.empty())
	{
		// 生成するオブジェクトがあるか確認する
		CheckCreateGameObject();

		// ゲームオブジェクト配列内のオブジェクトを解放する
		for (GameObject* object : game_object_list)
		{
			object->Finalize();
			delete object;
		}

		// 動的配列の解放
		game_object_list.clear();
	}
	
}

void GameObjectManager::CheckCreateGameObject()
{
	// 生成すべきゲームオブジェクトが無いなら処理を終了する
	if (create_list.empty())
	{
		return;
	}

	// ゲームオブジェクト配列内にZレイヤーを基準に挿入する（昇順）
	for (GameObject* target_object : create_list)
	{
		uint8_t z_layer = target_object->GetZLayer();
		auto iter = game_object_list.begin();
		for (; iter != game_object_list.end(); iter++)
		{
			if (z_layer < (*iter)->GetZLayer())
			{
				break;
			}
		}
		game_object_list.insert(iter, target_object);
	}

	// 動的配列の解放
	create_list.clear();
}

void GameObjectManager::CheckDestroyGameObject()
{
	// 破棄すべきゲームオブジェクトが無いなら処理を終了する
	if (destroy_list.empty())
	{
		return;
	}

	// ゲームオブジェクト配列内の対象のゲームオブジェクトを破棄する
	for (GameObject* target_object : destroy_list)
	{
		auto iter = game_object_list.begin();
		for (; iter != game_object_list.end(); iter++)
		{
			if (target_object == (*iter))
			{
				game_object_list.erase(iter);
				target_object->Finalize();
				delete target_object;
				break;
			}
		}
	}

	// 動的配列の解放
	destroy_list.clear();
}

void GameObjectManager::CheckHitCollision(GameObject* obj1, GameObject* obj2)
{
	// 存在しないオブジェクトであれば、処理を終了する（ヌルポチェック）
	if (obj1 == nullptr || obj2 == nullptr)
	{
		return;
	}

	// 一緒のオブジェクトであれば、処理を終了する
	if (obj1 == obj2)
	{
		return;
	}

	// コリジョン情報の取得
	Collision c1 = obj1->GetCollision();
	Collision c2 = obj2->GetCollision();

	// 当たり判定が有効なオブジェクトの組み合わせか確認する
	if (c1.IsHitTargetObject(c2.object_type) || c2.IsHitTargetObject(c1.object_type))
	{
		// 位置座標の差分を取得
		Vector2D diff = (obj2->GetLocation() + c2.pivot) - (obj1->GetLocation() + c1.pivot);

		// 矩形の大きさを取得
		Rect2D rect_size = (c1.rectangle_size + c2.rectangle_size) * 0.5f;

		// 矩形の大きさより差分が小さいならHit判定とする
		if (std::abs(diff.x) < rect_size.x && std::abs(diff.y) < rect_size.y)
		{
			// オブジェクトに当たったことを通知する
			obj1->OnHitCollision(obj2);
			obj2->OnHitCollision(obj1);
		}
	}
}
