#pragma once

#include "../SceneBase.h"
#include "../../Hero/Heros.h"

class PhaseTwo : public SceneBase
{
private:
    struct PhaseTwoHeros
    {
        HeroData data;
        bool is_selected;	// セレクトできるか
		bool is_alive;		// 死んでいるか

        // --- 追加: Updateで計算し、当たり判定と描画で使う変数 ---
        bool is_visible;        // 画面外でないか
        float draw_l, draw_r;   // 描画座標（左右）→ 当たり判定Xに使用
        float draw_u, draw_d;   // 描画座標（上下）→ 当たり判定Yに使用
        float src_x, src_w;     // 元画像の切り出し情報（描画用）
    };

	// ヒーローたち
    std::vector<PhaseTwoHeros> heros;
	// 場に出ているヒーロー
    std::vector<PhaseTwoHeros*> select_heros;
	// 合計パワー値
	int totalpower;

	// スクロール量
    float scrollx;

	// レスラー
	int wrestler_image;	// 画像
	int wrestler_power;	// 攻撃力
	int wrestler_count;	// 倒した数

	// 戦闘開始ボタン
	int start_image;	// 画像
	bool gameend;		// ゲームが終了したか


public:
	PhaseTwo()
		: SceneBase()
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
	// 未選択ヒーローを横スクロール用に配置する
	void layoutHeroes();
	// 当たり判定の確認/応答する処理
	void CollisionCheck();
};
