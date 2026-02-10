#pragma once

#include "../SceneBase.h"
#include "../../Hero/Heros.h"

#include <vector>

class PhaseTwo : public SceneBase
{
private:
	enum eAnimation
	{
		eNone,
		eBattle,
		eWin,
		eLose,
	};

	// ヒーローの情報と描画に関する構造体
    struct PhaseTwoHeros
    {
        HeroData data;
        bool is_selected;	// 選択中か
		bool is_delete;		// 消去するか
    };

	// ゲームシステム
	bool is_game_end;				// ゲームが終了しているかどうか
	eAnimation now_anime;			// 現在再生中のアニメーション
	eAnimation old_anime;			// 直前フレームで再生していたアニメーション
	float anime_second;				// 現在のアニメーションが開始してからの経過時間（秒）
	Vector2D start_button_position;	// 戦闘開始ボタンの描画座標
	Vector2D start_button_size;		// 戦闘開始ボタンのサイズ（半径）
	bool is_startbutton_hovered;	// 戦闘開始ボタンにカーソルがあるかどうか
	float old_mouse_x;				// 1フレーム前のマウスx座標

	// ヒーロー
    std::vector<PhaseTwoHeros> heros;			// 変身したヒーロー配列
    std::vector<PhaseTwoHeros*> select_heros;	// 戦闘するヒーロー配列
	int totalpower;								// 合計攻撃力

	// レスラー
	int wrestler_power;	// 攻撃力
	int wrestler_count;	// 倒した数
	int wrestler_rank;	// 敵のランク
	
	/* 画像 */
	int background_image;				// 背景画像
	int conveyer_image;					// ベルトコンベア
	int start_button_image[2];			// 戦闘開始ボタン画像
	int win_image;						// 勝利
	int lose_image;						// 敗北
	int wrestler_image[3];				// レスラー画像3種
	int wrestler_power_ui_image[3];		// 攻撃力背景画像（レスラー）
	int heros_power_ui_image;			// 総攻撃力背景画像（ヒーロー）
	std::vector<int> power_badge_image;	// 攻撃力のバッジ画像
	std::vector<int> power_number_image;// 攻撃力の数字画像

	/* サウンド */
	int bgn_battle_01;				// BGM_バトル1
	int bgn_battle_02;				// BGM_バトル2
	int bgn_battle_03;				// BGM_バトル3
	int se_battle_start;			// SE_開始
	int se_battle_lose;				// SE_敗北
	int se_battle_win;				// SE_勝利
	int voice_battle_enemy_entry;	// VOICE_レスラー入場
	int voice_battle_win;			// VOICE_ヒーロー勝利
	int voice_battle_lose;			// VOICE_ヒーロー敗北


public:
	PhaseTwo()
		: SceneBase()
		, is_game_end(false)
		, totalpower(0)
		, wrestler_image{0}
		, wrestler_power(0)
		, wrestler_count(0)
		, start_button_image{0}
		, is_startbutton_hovered(false)
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

private:
	// 当たり判定関数

	// 当たり判定チェック処理を呼び出す関数
	void CheckCollision();
	// スクロールキャラとの衝突チェック
	void CheckScrollCharacterCollision();
	// 選択解除の衝突チェック
	void CheckDeselectCollision();

private:
	// アニメーションが変わった時の処理
	void ProcessAnimationState();
	// アニメーション変更処理
	void ChangeAnimation(eAnimation next, float duration);
	// 次のレスラーにする処理
	void SetNextWrestler();
	// 合計攻撃力を計算する処理
	void SetTotalPower();
};
