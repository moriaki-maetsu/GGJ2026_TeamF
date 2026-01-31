#include "PhaseTwo.h"
#include "../../Utility/AssetContainer.h"
#include "../../Utility/InputManager.h"
#include <DxLib.h>

void PhaseTwo::Initialize()
{
	int red = AssetContainer::Get()->GetImages("character_red_02.png")[0];
	int blue = AssetContainer::Get()->GetImages("character_blue_02.png")[0];
	int green = AssetContainer::Get()->GetImages("character_green_02.png")[0];
	int pink = AssetContainer::Get()->GetImages("character_pink_02.png")[0];

    int i = 0;
    for (int j = 0; j < 4; j++)
    {
        heros.push_back({ { Vector2D{100.f + 200.f * i++,100.0f},eColor::eRed,i,red }, false });
        heros.push_back({ { Vector2D{100.f + 200.f * i++,100.0f},eColor::eBlue,i,blue }, false });
        heros.push_back({ { Vector2D{100.f + 200.f * i++,100.0f},eColor::eGreen,i,green }, false });
        heros.push_back({ { Vector2D{100.f + 200.f * i++,100.0f},eColor::ePink,i,pink }, false });
    }

    // レスラー
	wrestler_image = AssetContainer::Get()->GetImages("ProtoWrestler.png")[0];
    wrestler_power = 5;

    // 戦闘ボタン
    start_image = AssetContainer::Get()->GetImages("ProtoVSUI.png")[0]; // 128*128(仮)
}

eSceneType PhaseTwo::Update(float delta_second)
{
    layoutHeroes();
    CollisionCheck();

    if (gameend)
    {
        return eSceneType::eResult;
    }
    
    return GetNowSceneType();
}


void PhaseTwo::Draw() const
{
    const float left_limit = 200.0f;
    const float right_limit = 1000.0f;
    const float img_h = 400.0f;

    DrawLineAA(left_limit, 0.0f, left_limit, HERO_SIZE_Y * 2.0f, 0xFFFFFF, 5.0f);
    DrawLineAA(right_limit, 0.0f, right_limit, HERO_SIZE_Y * 2.0f, 0xFFFFFF, 5.0f);

    // ヒーロー選択スクロールの描画
    for (int i = 0; i < heros.size(); ++i) {
        const auto& hero = heros[i];

        if (hero.is_selected || !hero.is_visible) continue;

        DrawRectExtendGraphF(
            hero.draw_l, hero.draw_u,
            hero.draw_r, hero.draw_d,
            hero.src_x, 0.0f,
            hero.src_w, img_h,
            hero.data.image, TRUE
        );

        DrawFormatStringF(hero.draw_l, hero.draw_d + 5.0f, 0xFFFF00, "Index: %d", i);
        DrawFormatStringF(hero.draw_l, hero.draw_d + 20.0f, 0xFFFF00, "Power: %d", hero.data.power);
        DrawBoxAA(hero.draw_l, hero.draw_u, hero.draw_r, hero.draw_d, 0xFFFFFF, FALSE);
    }

    // 場に出ているのヒーローの描画
    for (size_t i = 0; i < select_heros.size(); i++)
    {
        const PhaseTwoHeros* select = select_heros[i];
        Vector2D position{ 100.0f * (i + 1), 500.0f };
        DrawRotaGraphF(position.x, position.y, 0.5f, 0.0f, select->data.image, TRUE);
    }
    DrawFormatStringF(150.0f, 400.0f, 0xffffff, "合計パワー:%d", totalpower);

    // レスラーの描画
    DrawRotaGraphF(1000.0f, 500.0f, 1.f, 0.0f, wrestler_image, TRUE);
    DrawFormatStringF(1000.0f, 400.0f, 0xffffff, "レスラーパワー:%d", wrestler_power);

    // 戦闘スタートボタン
    DrawRotaGraphF(640.0f, 360.0f, 1.f, 0.0f, start_image, FALSE);
}

void PhaseTwo::Finalize()
{
}

void PhaseTwo::layoutHeroes()
{
    InputManager* input = InputManager::Get();

    const float HERO_INTERVAL = 200.0f;   // 基準間隔（変えない）
    const float left_limit = 200.0f;
    const float right_limit = 1000.0f;
    const float img_w = 300.0f;
    const float SCROLL_SPEED = 5.0f;

    // --- 選択数カウント ---
    int selected_count = 0;
    for (const auto& h : heros)
        if (!h.is_selected) selected_count++;

    if (selected_count > 0)
    {
        const int HERO_MAX = static_cast<int>(heros.size());

        // ★ ループ幅は固定
        const float TOTAL_LOOP_WIDTH = HERO_MAX * HERO_INTERVAL;

        // ★ 表示間隔だけを歪める
        float logical_interval = HERO_INTERVAL;
        logical_interval *= static_cast<float>(HERO_MAX) / selected_count;

        // --- スクロール（速度固定） ---
        float rot = -input->GetScrollWheel().y * SCROLL_SPEED * 10.0f;
        scrollx += rot;

        if (scrollx < 0.0f)
            scrollx += TOTAL_LOOP_WIDTH;

        if (scrollx >= TOTAL_LOOP_WIDTH)
            scrollx -= TOTAL_LOOP_WIDTH;

        // --- 座標計算 ---
        int drawn_count = 0;

        for (auto& hero : heros)
        {
            hero.is_visible = false;
            if (hero.is_selected) continue;

            // ★ ここで歪めた間隔を使用
            float base_x = drawn_count * logical_interval;
            float raw_left = base_x - HERO_SIZE_X - scrollx;

            float looped_left = fmodf(raw_left, TOTAL_LOOP_WIDTH);
            if (looped_left < 0.0f)
                looped_left += TOTAL_LOOP_WIDTH;

            float left = looped_left;
            float right = looped_left + (HERO_SIZE_X * 2);
            float up = hero.data.position.y - HERO_SIZE_Y;
            float down = hero.data.position.y + HERO_SIZE_Y;

            if (right <= left_limit || left >= right_limit)
            {
                drawn_count++;
                continue;
            }

            hero.is_visible = true;

            hero.draw_l = left;
            hero.draw_r = right;
            hero.draw_u = up;
            hero.draw_d = down;
            hero.src_x = 0.0f;
            hero.src_w = img_w;

            float display_w = right - left;

            // 左クリップ
            if (left < left_limit)
            {
                float cut_px = left_limit - left;
                float src_cut = cut_px * (img_w / display_w);
                hero.draw_l = left_limit;
                hero.src_x = src_cut;
                hero.src_w -= src_cut;
            }

            // 右クリップ
            if (right > right_limit)
            {
                float cut_px = right - right_limit;
                float src_cut = cut_px * (img_w / display_w);
                hero.draw_r = right_limit;
                hero.src_w -= src_cut;
            }

            drawn_count++;
        }
    }
}

void PhaseTwo::CollisionCheck()
{
    InputManager* input = InputManager::Get();

    // マウスが押された時の処理
    if (input->GetMouseState(MOUSE_INPUT_LEFT) == eInputState::eClick)
    {
        float mx = input->GetMouseLocation().x;
        float my = input->GetMouseLocation().y;

        // スクロールのヒーローが押されたか判定
        for (auto& hero : heros)
        {
            if (hero.is_visible &&
                mx >= hero.draw_l && mx <= hero.draw_r &&
                my >= hero.draw_u && my <= hero.draw_d)
            {
                bool already_exists = false;

                // 同じ色がいるか確認
                for (const PhaseTwoHeros* select : select_heros)
                {
                    if (select->data.color == hero.data.color)
                    {
                        already_exists = true;
                        break;
                    }
                }

                // 同じ色がいないなら
                if (!already_exists)
                {
                    select_heros.push_back(&hero);
                    hero.is_selected = true;

                    int total = 0;
                    for (const PhaseTwoHeros* select : select_heros)
                    {
                        total += select->data.power;
                    }
                    totalpower = total;
                }
            }
        }

        // vsが押されたか判定
        if (mx >= 640.0f - 128.0f && mx <= 640.0f + 128.0f &&
            my >= 360.0f - 128.0f && my <= 360.0f + 128.0f)
        {
            if (totalpower > wrestler_power)
            {
                // 勝ち
                wrestler_count++;
                wrestler_power += 10;

                for (int i = 0; i < select_heros.size(); i++)
                {
                    // 消したい対象のポインタ (例)
                    PhaseTwoHeros* target = select_heros[i];

                    // 削除処理
                    select_heros.erase(
                        std::remove(select_heros.begin(), select_heros.end(), target),
                        select_heros.end()
                    );
                }
                select_heros.clear();
            }
            else
            {
                // 負け
                gameend = true;
            }
        }
    }
}