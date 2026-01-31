#pragma once

#include "../SceneBase.h"
#include "../../Hero/Heros.h"

class PhaseTwo : public SceneBase
{
private:
	// ヒーローの情報と描画に関する構造体
    struct PhaseTwoHeros
    {
        HeroData data;
        bool is_selected;	// セレクトできるか
		bool is_delete;		// 消去するか

        // --- 追加: Updateで計算し、当たり判定と描画で使う変数 ---
        bool is_visible;        // 画面外でないか
        float draw_l, draw_r;   // 描画座標（左右）→ 当たり判定Xに使用
        float draw_u, draw_d;   // 描画座標（上下）→ 当たり判定Yに使用
        float src_x, src_w;     // 元画像の切り出し情報（描画用）
    };

	// システム
	bool gameend;		// ゲームが終了フラグ
	float scrollx;		// スクロール量

	// ヒーロー
    std::vector<PhaseTwoHeros> heros;			// 変身したヒーロー
    std::vector<PhaseTwoHeros*> select_heros;	// 戦闘するヒーロー
	int totalpower;								// 合計パワー

	// レスラー
	int wrestler_image;	// 画像
	int wrestler_power;	// 攻撃力
	int wrestler_count;	// 倒した数
	
	// ボタン
	int start_image;	// 戦闘開始ボタン画像

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
	// 当たり判定の確認/応答する処理
	void CollisionCheck();
	// 次のレスラーにする処理
	void SetNextWrestler();
	// 合計パワーを計算する処理
	void SetTotalPower();
};
