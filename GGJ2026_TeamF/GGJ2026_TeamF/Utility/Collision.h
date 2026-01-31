#pragma once

#include "Geometry.h"
#include <vector>

// オブジェクト列挙体クラス
enum class eObjectType
{
	eNone,
	ePlayer,
	eGround,
	eBullets,
	eWall,
	eOther,
};

// 矩形コリジョンクラス
class Collision
{
public:
	// 当たり判定の中心（相対座標）
	Location2D	pivot = 0.0f;
	// 矩形の大きさ
	Rect2D rectangle_size = 0.0f;
	// 自身のオブジェクトタイプ
	eObjectType	object_type = eObjectType::eNone;
	// 当たり判定を有効にするオブジェクトタイプ
	std::vector<eObjectType> hit_object_type;

public:
	Collision() = default;
	~Collision()
	{
		// 動的配列内に要素が存在していたら、解放してあげる
		if (not hit_object_type.empty())
		{
			hit_object_type.clear();
		}
	}

public:
	// 当たり判定有効オブジェクト確認処理
	bool IsHitTargetObject(eObjectType hit_object_type) const
	{
		// 有効にするオブジェクトかどうか確認する
		for (const eObjectType& type : this->hit_object_type)
		{
			if (type == hit_object_type)
			{
				// 有効なオブジェクトタイプであることを通知
				return true;
			}
		}

		// 有効じゃないことを通知する
		return false;
	}

};
