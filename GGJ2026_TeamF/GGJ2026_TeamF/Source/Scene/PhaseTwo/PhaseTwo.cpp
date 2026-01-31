#include "PhaseTwo.h"
#include "../../Utility/AssetContainer.h"
#include "../../Utility/InputManager.h"
#include <DxLib.h>

void PhaseTwo::Initialize()
{
	int handle = AssetContainer::Get()->GetImages("character_blue_01.png")[0];
	for (int i = 0; i < 10; i++)
	{
		heros.push_back({ { Vector2D{100.f + 200.f * i,100.0f},eColor::eRed,10,handle }, true });
	}
}

eSceneType PhaseTwo::Update(float delta_second)
{
	InputManager* input = InputManager::Get();
    const float HERO_INTERVAL = 200.0f;
    const float TOTAL_LOOP_WIDTH = heros.size() * HERO_INTERVAL;

    if (input->GetKeyState(KEY_INPUT_RIGHT) == eInputState::ePressed)
    {
        scrollx += 5.0f;
    }
    if (input->GetKeyState(KEY_INPUT_LEFT) == eInputState::ePressed)
    {
        scrollx -= 5.0f;
    }

    if (scrollx < 0)
    {
        scrollx += TOTAL_LOOP_WIDTH;
    }
    if (scrollx >= TOTAL_LOOP_WIDTH)
    {
        scrollx -= TOTAL_LOOP_WIDTH;
    }
	return GetNowSceneType();
}

void PhaseTwo::Draw() const
{
    // --- 描画制限エリアとループ幅の設定 ---
    const float left_limit = 200.0f;
    const float right_limit = 1000.0f;
    const float HERO_INTERVAL = 200.0f;
    const float TOTAL_LOOP_WIDTH = heros.size() * HERO_INTERVAL;

    DrawLineAA(left_limit, 0.0f, left_limit, 1000.0f, 0xFFFFFF, 5.0f);
    DrawLineAA(right_limit, 0.0f, right_limit, 1000.0f, 0xFFFFFF, 5.0f);

    for (int i = 0; i < heros.size(); ++i) {
        const auto& hero = heros[i];
        if (!hero.is_selected) continue;

        // --- スクロールを考慮した基本の左端座標 ---
        float raw_left = hero.data.position.x - HERO_SIZE_X - scrollx;

        // --- 画面外に出た際のループ位置補正 ---
        float looped_left = fmodf(raw_left, TOTAL_LOOP_WIDTH);
        if (looped_left < 0) looped_left += TOTAL_LOOP_WIDTH;

        // --- 描画用の基本矩形情報（補正後） ---
        float left = looped_left;
        float right = looped_left + (HERO_SIZE_X * 2);
        float up = hero.data.position.y - HERO_SIZE_Y;
        float down = hero.data.position.y + HERO_SIZE_Y;

        // --- 描画制限エリア外ならスキップ ---
        if (right <= left_limit || left >= right_limit) continue;

        // --- 元画像のサイズと描画パラメータの初期化 ---
        const float img_w = 300.0f;
        const float img_h = 400.0f;
        float draw_l = left;
        float draw_r = right;
        float src_x = 0.0f;
        float src_w = img_w;
        float display_w = right - left; // 表示領域の幅

        // --- 左側境界によるクリッピング処理 ---
        if (left < left_limit) {
            // はみ出し量と、それに対応する元画像の削り幅を算出
            float cut_px = left_limit - left;
            float src_cut = cut_px * (img_w / display_w);

            // 描画開始位置と元画像参照位置を調整
            draw_l = left_limit;
            src_x = src_cut;
            src_w -= src_cut;
        }

        // --- 右側境界によるクリッピング処理 ---
        if (right > right_limit) {
            // はみ出し量と、それに対応する元画像の削り幅を算出
            float cut_px = right - right_limit;
            float src_cut = cut_px * (img_w / display_w);

            // 描画終了位置と切り出し横幅を調整
            draw_r = right_limit;
            src_w -= src_cut;
        }

        // --- 最終的な描画実行とデバッグ情報の表示 ---
        DrawRectExtendGraphF(draw_l, up, draw_r, down, src_x, 0.0f, src_w, img_h, hero.data.image, TRUE);
        DrawFormatStringF(draw_l, down + 5.0f, 0xFFFF00, "Index: %d", i);
        DrawBoxAA(draw_l, up, draw_r, down, 0xFFFFFF, FALSE);
    }
}


void PhaseTwo::Finalize()
{
}



