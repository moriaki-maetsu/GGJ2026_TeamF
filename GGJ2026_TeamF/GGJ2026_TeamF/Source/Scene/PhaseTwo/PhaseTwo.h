#pragma once

#include "../SceneBase.h"
#include "../../Hero/Heros.h"

#include <vector>

class PhaseTwo : public SceneBase
{
private:
	// ヒーローの情報と描画に関する構造体
    struct PhaseTwoHeros
    {
        HeroData data;
        bool is_selected;	// 選択中か
		bool is_delete;		// 消去するか
    };

	// システム
	bool gameend;		// ゲーム終了フラグ
	float scrollx;		// スクロール量

	// ヒーロー
    std::vector<PhaseTwoHeros> heros;			// 変身したヒーロー
    std::vector<PhaseTwoHeros*> select_heros;	// 戦闘するヒーロー
	std::vector<int> power_badge_image;			// パワーの画像
	int totalpower;								// 合計パワー

	// レスラー
	int wrestler_image;	// 画像
	int wrestler_power;	// 攻撃力
	int wrestler_count;	// 倒した数
	
	// ボタン
	int start_image;			// 戦闘開始ボタン画像
	Vector2D start_position;	// 描画座標
	Vector2D start_size;		// ボタンの大きさ

	// 背景
	int background;	// 背景画像

public:
	PhaseTwo()
		: SceneBase()
		, gameend(false)
		, scrollx(0.0f)
		, totalpower(0)
		, wrestler_image(0)
		, wrestler_power(0)
		, wrestler_count(0)
		, start_image(0)
	{

	}
	virtual ~PhaseTwo() = default;

public:
	// 初期化
	virtual void Initialize() override;
	// 更新
	virtual eSceneType Update(float delta_second) override;
	// 描画
	virtual void Draw() const override;
	// 終了時
	virtual void Finalize() override;

public:
	virtual eSceneType GetNowSceneType() const override
	{
		return eSceneType::ePhaseTwo;
	}

private:
	// 未選択ヒーローを横スクロール用に配置する処理
	void layoutHeroes();

public:
	// 当たり判定関数

	// 当たり判定チェック処理を呼び出す関数
	void CheckCollision();
	// スクロールキャラとの衝突チェック
	void CheckScrollCharacterCollision();
	// 選択解除の衝突チェック
	void CheckDeselectCollision();

public:

	// 次のレスラーにする処理
	void SetNextWrestler();
	// 合計パワーを計算する処理
	void SetTotalPower();
};
