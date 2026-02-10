#include "PhaseTwo.h"
#include "../../Utility/AssetContainer.h"
#include "../../Utility/InputManager.h"
#include "../../Hero/Heros.h"
#include "../Result/ResultData.h"

#include <cmath>
#include <DxLib.h>

#define HERO_SPACE (100.0f)     // ヒーロー同士の間隔

void PhaseTwo::Initialize()
{
    // フェーズ1で生成されたヒーロー一覧を取得
    const std::vector<HeroData>& raw_data = Heros::Get()->GetHeros();

    // フェーズ2用ヒーロー配列を初期化（中身を空にし、必要分の容量を確保）
    heros.clear();
    heros.reserve(raw_data.size());

    // フェーズ1のヒーローデータを元に、
    // フェーズ2で使用する管理フラグ（bool×2）を追加して配列を構築
    for (const HeroData& data : raw_data)
    {
        heros.push_back({ data, false, false });
    }

    AssetContainer* container = AssetContainer::Get();

#if _DEBUG  // Debugの時のみ

    // 配列が空ならヒーローを追加する
    //if (raw_data.empty())
    //{
    //    int red = container->GetImages("character_red_02.png")[0];
    //    int blue = container->GetImages("character_blue_02.png")[0];
    //    int green = container->GetImages("character_green_02.png")[0];
    //    int pink = container->GetImages("character_pink_02.png")[0];
    //    int yellow = container->GetImages("character_yellow_02.png")[0];
    //    for (int i = 0; i < 10; i++)
    //    {
    //        heros.push_back({ { Vector2D{0.0f,0.0f},eColor::eRed,10,red }, false, false });
    //        heros.push_back({ { Vector2D{0.0f,0.0f},eColor::eBlue,10,blue }, false, false });
    //        heros.push_back({ { Vector2D{0.0f,0.0f},eColor::eYellow,10,yellow }, false, false });
    //        heros.push_back({ { Vector2D{0.0f,0.0f},eColor::eGreen,10,green }, false, false });
    //        heros.push_back({ { Vector2D{0.0f,0.0f},eColor::ePink,10,pink }, false, false });
    //    }
    //}
# endif

    // ヒーローの座標を初期化
    for (int i = 0; i < heros.size(); i++)
    {
        heros[i].data.position = { HERO_SIZE_X + i * HERO_SPACE,80.0f };
    }

    start_button_position = { 640.0f,650.0f }; // 戦闘開始ボタンの座標
    start_button_size = { 150.0f,40.0f };      // 戦闘開始ボタンの大きさ（半径）
    now_anime = eAnimation::eNone;      // 今のアニメーション
    old_anime = eAnimation::eNone;      // 1つ前のアニメーション
    anime_second = 0.0f;                // アニメーションを行う時間
    wrestler_power = 5;                 // レスラー攻撃力
    wrestler_rank = 0;                  // レスラーランク

    /* 画像の読み込み */
    // システム
    background_image = container->GetImages("bg_battle_01.png")[0];                               // 戦闘フェーズ背景
    conveyer_image = container->GetImages("conveyer.png")[0];                               // ベルトコンベア
    power_number_image = container->GetImages("ui_number_02.png", 11, 11, 1, 34, 68);       // 数字
    win_image = container->GetImages("ui_battle_win.png")[0];                               // 勝利
    lose_image = container->GetImages("ui_battle_lose.png")[0];                             // 敗北
    start_button_image[0] = container->GetImages("ui_battle_button_start_01.png")[0];       // 戦闘開始ボタン
    start_button_image[1] = container->GetImages("ui_battle_button_start_02.png")[0];       // 戦闘開始ボタン（カーソルがあった時）

    // ヒーロー
    power_badge_image = container->GetImages("ui_power_badge.png", 10, 10, 1, 120, 100);    // 攻撃力バッジ
    heros_power_ui_image = container->GetImages("ui_power_plate_player.png")[0];            // 総攻撃力背景UI

    // レスラー
    wrestler_image[0] = container->GetImages("enemy_01.png")[0];                            // レスラー1
    wrestler_image[1] = container->GetImages("enemy_02.png")[0];                            // レスラー2
    wrestler_image[2] = container->GetImages("enemy_03.png")[0];                            // レスラー3
    wrestler_power_ui_image[0] = container->GetImages("ui_power_plate_enemy_01.png")[0];    // 攻撃力背景画像1
    wrestler_power_ui_image[1] = container->GetImages("ui_power_plate_enemy_02.png")[0];    // 攻撃力背景画像2
    wrestler_power_ui_image[2] = container->GetImages("ui_power_plate_enemy_03.png")[0];    // 攻撃力背景画像3

    /* サウンド読み込み */
    bgn_battle_01 = container->GetSound("bgn_battle_01.mp3");
    bgn_battle_02 = container->GetSound("bgn_battle_02.mp3");
    bgn_battle_03 = container->GetSound("bgn_battle_03.mp3");
    se_battle_lose = container->GetSound("se_battle_lose.mp3");
    se_battle_start = container->GetSound("se_battle_start.mp3");
    se_battle_win = container->GetSound("se_battle_win.mp3");
    voice_battle_enemy_entry = container->GetSound("voice_battle_enemy_entry.mp3");
    voice_battle_lose = container->GetSound("voice_battle_lose.mp3");
    voice_battle_win = container->GetSound("voice_battle_win.mp3");

    /* 音量調整 */
    ChangeVolumeSoundMem(255 * 35 / 100, bgn_battle_01);
    ChangeVolumeSoundMem(255 * 35 / 100, bgn_battle_02);
    ChangeVolumeSoundMem(255 * 35 / 100, bgn_battle_03);
    ChangeVolumeSoundMem(255 * 50 / 100, se_battle_lose);
    ChangeVolumeSoundMem(255 * 40 / 100, se_battle_start);
    ChangeVolumeSoundMem(255 * 50 / 100, se_battle_win);
    ChangeVolumeSoundMem(255 * 60 / 100, voice_battle_enemy_entry);
    ChangeVolumeSoundMem(255 * 60 / 100, voice_battle_lose);
    ChangeVolumeSoundMem(255 * 50 / 100, voice_battle_win);

    // サウンドを流す
    PlaySoundMem(se_battle_start, DX_PLAYTYPE_BACK);    // 着地音（ズドォン）
    PlaySoundMem(bgn_battle_01, DX_PLAYTYPE_LOOP);

    // ヒーローがいなかったら
    if (heros.empty())
    {
        // バトルして敗北
        ChangeAnimation(eAnimation::eBattle, 0.1f);
    }
}

eSceneType PhaseTwo::Update(float delta_second)
{
    if (anime_second > 0.001f)
    {
        // アニメーション待ち時間を消費
        anime_second -= delta_second;
    }
    else if (old_anime != now_anime)
    {
        // アニメーション状態が変わったときの処理
        old_anime = now_anime;
        ProcessAnimationState();
    }

    // スクロール移動処理
    InputManager* input = InputManager::Get();
    // マウスホイール
    float rot = -input->GetScrollWheel().y * 50.0f;
    // ドラッグでも移動可能
    if (input->GetMouseState(MOUSE_INPUT_LEFT) == eInputState::eClick)
    {
        old_mouse_x = input->GetMouseLocation().x;
    }
    else if (input->GetMouseState(MOUSE_INPUT_LEFT) == eInputState::ePressed)
    {
        float now_mouse_x = input->GetMouseLocation().x;
        rot -= old_mouse_x - now_mouse_x;
        old_mouse_x = now_mouse_x;
    }

    // 当たり判定の確認処理
    CheckCollision();

    // バッジの大きさ
    constexpr float BADGE_W = 40.0f;
    const float raw_loop = (float)heros.size() * HERO_SPACE;
    const float min_loop = 1280.0f + 2.0f * HERO_SIZE_X + BADGE_W;
    const float total_loop = (raw_loop < min_loop) ? min_loop : raw_loop;

    const float left_bound = -(HERO_SIZE_X + BADGE_W);

    for (int i = 0; i < (int)heros.size(); i++)
    {
        float x = heros[i].data.position.x + rot;

        x = left_bound + std::fmod(x - left_bound, total_loop);
        if (x < left_bound) x += total_loop;

        heros[i].data.position.x = x;
    }

    if (is_game_end)
    {
        ResultData::Get()->SetWrestlerCount(wrestler_count);
        return eSceneType::eResult;
    }

    return GetNowSceneType();
}

void PhaseTwo::Draw() const
{
    // 背景の描画
    DrawRotaGraphF(640.0f, 360.0f, 1.f, 0.0f, background_image, TRUE);
    // ベルトコンベア
    DrawRotaGraphF(640.0f, 80.0f, 0.35f, 0.0f, conveyer_image, TRUE);

    // ヒーロー選択スクロールの描画
    for (int i = 0; i < heros.size(); ++i) {
        const PhaseTwoHeros& hero = heros[i];

        if (hero.is_selected) continue;

        Vector2D p = hero.data.position;

        DrawRotaGraphF(p.x, p.y, 1.0f, 0.0f, hero.data.image, TRUE);
        // 強さの描画
        Vector2D badgeoffset = { -20.0f,-30.0f };   // 少しずらす
        DrawRotaGraphF(p.x + HERO_SIZE_X + badgeoffset.x, p.y + HERO_SIZE_Y + badgeoffset.y, 0.7f, 0.0f, power_badge_image[hero.data.power - 1], TRUE);

        // 配列の何番目か
        //DrawFormatStringF(p.x, p.y + 100.0f, 0xffffff, "%d", i);

        // 当たり判定
        //DrawBoxAA(hero.data.position.x - HERO_SIZE_X, hero.data.position.y - HERO_SIZE_Y, hero.data.position.x + HERO_SIZE_X, hero.data.position.y + HERO_SIZE_Y, 0xFFFFFF, FALSE);
        //DrawBoxAA(hero.data.position.x - HERO_SIZE_X, hero.data.position.y - HERO_SIZE_Y, hero.data.position.x + HERO_SIZE_X+40, hero.data.position.y + HERO_SIZE_Y, GetColor(255,0,0), FALSE);
    }

    // 敗北アニメーション中じゃないなら
    if (now_anime != eAnimation::eLose)
    {
        // 総攻撃力の背景UI
        DrawRotaGraphF(400.0f, 270.0f, 1.0f, 0.0f, heros_power_ui_image, TRUE);

        // 総攻撃力を描画
        int temp_power = totalpower;
        float draw_x = 480.0f; // 描画の基準位置（1の位の位置）
        float spacing = 16.0f; // 数字同士の間隔
        do {
            int digit = temp_power % 10;

            DrawRotaGraphF(draw_x, 275.0f, 0.5f, 0.0f, power_number_image[digit], TRUE);

            draw_x -= spacing;
            temp_power /= 10;

        } while (temp_power > 0);

        // 場に出ているのヒーローの描画
        for (size_t i = 0; i < select_heros.size(); i++)
        {
            const PhaseTwoHeros* select = select_heros[i];
            Vector2D position = { 0.0f,0.0f };

            Vector2D center = { 500.0f,500.0f };

            switch (i)
            {
            case 0:
                position = center;
                break;
            case 1:
                position = { center.x - 120,center.y - 60 };
                break;
            case 2:
                position = { center.x - 120,center.y + 60 };
                break;
            case 3:
                position = { center.x - 240,center.y - 120 };
                break;
            case 4:
                position = { center.x - 240,center.y + 120 };
                break;
            }
            // ヒーローを
            DrawRotaGraphF(position.x, position.y, 1.2f, 0.0f, select->data.image, TRUE);
            // バッジ
            Vector2D badgeoffset = { -20.0f,-30.0f };
            DrawRotaGraphF(position.x + HERO_SIZE_X + badgeoffset.x, position.y + HERO_SIZE_Y + badgeoffset.y, 0.84f, 0.0f, power_badge_image[select->data.power - 1], TRUE);
            // 当たり判定
            //DrawBoxAA(position.x - HERO_SIZE_X, position.y - HERO_SIZE_Y, position.x + HERO_SIZE_X, position.y + HERO_SIZE_Y, 0xFFFFFF, FALSE);
        }
    }

    // 勝利アニメーション中じゃないなら
    if (now_anime != eAnimation::eWin)
    {
        // レスラーの攻撃力背景UI
        DrawRotaGraphF(900.0f, 270.0f, 1.0f, 0.0f, wrestler_power_ui_image[wrestler_rank], TRUE);

        // レスラーの攻撃力を描画
        int temp_power = wrestler_power;
        float draw_x = 980.0f; // 描画の基準位置（1の位の位置）
        float spacing = 16.0f; // 数字同士の間隔

        do {
            int digit = temp_power % 10;
            DrawRotaGraphF(draw_x, 290.0f, 0.5f, 0.0f, power_number_image[digit], TRUE);
            draw_x -= spacing;
            temp_power /= 10;

        } while (temp_power > 0);

        // レスラーの描画
        DrawRotaGraphF(900.0f, 500.0f, 0.5f, 0.0f, wrestler_image[wrestler_rank], TRUE);
    }

    // アニメーションの状態描画
    switch (now_anime)
    {
    case eNone:
    case eBattle:
        // 戦闘開始ボタン
        DrawRotaGraphF(start_button_position.x, start_button_position.y, 1.f, 0.0f, start_button_image[is_startbutton_hovered ? 1 : 0], TRUE);
        break;
    case eWin:
        // 勝利UI
        DrawRotaGraphF(start_button_position.x, 400.0f, 1.f, 0.0f, win_image, TRUE);
        break;
    case eLose:
        // 敗北UI
        DrawRotaGraphF(start_button_position.x, 400.0f, 1.f, 0.0f, lose_image, TRUE);
        break;
    }
}

void PhaseTwo::Finalize()
{
    // BGMを止める
    StopSoundMem(bgn_battle_01);
    StopSoundMem(bgn_battle_02);
    StopSoundMem(bgn_battle_03);
}

void PhaseTwo::CheckCollision()
{
    // アニメーション中じゃないなら
    if (now_anime == eAnimation::eNone)
    {
        InputManager* input = InputManager::Get();
        Vector2D mouse = input->GetMouseLocation();
        is_startbutton_hovered = false;

        if (mouse.x >= start_button_position.x - start_button_size.x && mouse.x <= start_button_position.x + start_button_size.x &&
            mouse.y >= start_button_position.y - start_button_size.y && mouse.y <= start_button_position.y + start_button_size.y)
        {
            is_startbutton_hovered = true;
        }

        // マウスが押された時の処理
        if (input->GetMouseState(MOUSE_INPUT_LEFT) == eInputState::eClick)
        {
            // スクロールキャラクターの判定を行う
            CheckScrollCharacterCollision();
            // 場に出ているキャラクターの判定を行う
            CheckDeselectCollision();
            // vsが押されたか判定
            if (mouse.x >= start_button_position.x - start_button_size.x && mouse.x <= start_button_position.x + start_button_size.x &&
                mouse.y >= start_button_position.y - start_button_size.y && mouse.y <= start_button_position.y + start_button_size.y)
            {
                ChangeAnimation(eAnimation::eBattle, 0.1f);
            }

            // 合計攻撃力を再計算
            SetTotalPower();
        }
    }
}

void PhaseTwo::CheckScrollCharacterCollision()
{
    // スクロールのヒーローが押されたか判定
    for (auto& hero : heros)
    {
        if (hero.is_selected) continue;

        float left = hero.data.position.x - HERO_SIZE_X;
        float right = hero.data.position.x + HERO_SIZE_X;
        float up = hero.data.position.y - HERO_SIZE_Y;
        float down = hero.data.position.y + HERO_SIZE_Y;

        InputManager* input = InputManager::Get();
        Vector2D mouse = input->GetMouseLocation();
        // 矢印が重なっていたら
        if (mouse.x >= left && mouse.x <= right &&
            mouse.y >= up && mouse.y <= down)
        {
            // --- (既存のフラグ解除と削除処理はそのまま) ---
            for (PhaseTwoHeros* s : select_heros)
            {
                if (s->data.color == hero.data.color) s->is_selected = false;
            }

            select_heros.erase(
                std::remove_if(select_heros.begin(), select_heros.end(),
                    [&](const PhaseTwoHeros* s) { return s->data.color == hero.data.color; }),
                select_heros.end()
            );

            // 新しく追加
            select_heros.push_back(&hero);
            hero.is_selected = true;

            // 色の優先順位を定義するラムダ関数
            auto getColorPriority = [](eColor color) {
                switch (color) {
                case eColor::eRed:    return 1;
                case eColor::eBlue:   return 2;
                case eColor::eYellow: return 3;
                case eColor::eGreen:  return 4;
                case eColor::ePink:   return 5;
                default:              return 99; // その他は後ろへ
                }
                };

            // バブルソート：ポインタの並び順を入れ替える
            int n = (int)select_heros.size();
            for (int i = 0; i < n - 1; ++i) {
                for (int j = 0; j < n - i - 1; ++j) {
                    // 隣り合う要素の優先度を比較
                    if (getColorPriority(select_heros[j]->data.color) >
                        getColorPriority(select_heros[j + 1]->data.color)) {

                        // 要素（ポインタ）を入れ替える
                        PhaseTwoHeros* temp = select_heros[j];
                        select_heros[j] = select_heros[j + 1];
                        select_heros[j + 1] = temp;
                    }
                }
            }
        }
    }
}

void PhaseTwo::CheckDeselectCollision()
{
    // 場に出ているヒーローをクリックしたら、そのヒーローを場から外す（select_heros から削除）
    InputManager* input = InputManager::Get();
    Vector2D mouse = input->GetMouseLocation();

    int i = 0; // カウンターを用意
    select_heros.erase(
        std::remove_if(select_heros.begin(), select_heros.end(), [&](PhaseTwoHeros* hero) {
            // ヒーローの座標
            Vector2D center = { 500.0f,500.0f };
            Vector2D position = { 0.0f,0.0f };
            switch (i)
            {
            case 0:
                position = center;
                break;
            case 1:
                position = { center.x - 120,center.y - 60 };
                break;
            case 2:
                position = { center.x - 120,center.y + 60 };
                break;
            case 3:
                position = { center.x - 240,center.y - 120 };
                break;
            case 4:
                position = { center.x - 240,center.y + 120 };
                break;
            }

            // 判定が終わる前にカウンターを進める
            i++;

            // 計算した position を使って当たり判定
            Vector2D lu = { position.x - HERO_SIZE_X, position.y - HERO_SIZE_Y };
            Vector2D rd = { position.x + HERO_SIZE_X, position.y + HERO_SIZE_Y };

            // クリックされたか判定
            if (mouse.x >= lu.x && mouse.x <= rd.x && mouse.y >= lu.y && mouse.y <= rd.y)
            {
                // フラグを戻す
                hero->is_selected = false;
                return true; // 削除
            }
            return false; // 保持
            }),
        select_heros.end()
    );
}

void PhaseTwo::ProcessAnimationState()
{
    switch (now_anime)
    {
    case eNone:
        ChangeAnimation(eAnimation::eNone, 0.0f);
        select_heros.clear();
        totalpower = 0;

        // 死んだヒーローを配列から消去
        heros.erase(
            std::remove_if(heros.begin(), heros.end(),
                [](const PhaseTwoHeros& h) {
                    return h.is_delete;
                }
            ),
            heros.end()
        );

        // ヒーローがいなかったら
        if (heros.empty())
        {
            // バトルして敗北
            ChangeAnimation(eAnimation::eBattle, 0.1f);
        }
        else
        {
            // ヒーローがまだいたらSE
            PlaySoundMem(voice_battle_enemy_entry, DX_PLAYTYPE_BACK);   // （これは強敵だな）
            // ヒーローの座標を初期化
            for (int i = 0; i < heros.size(); i++)
            {
                heros[i].data.position = { HERO_SIZE_X + i * HERO_SPACE,80.0f };
            }
        }

        break;

    case eBattle:
        if (totalpower >= wrestler_power)
        {
            // 勝ち
            SetNextWrestler();
            wrestler_count++;
            ChangeAnimation(eAnimation::eWin, 2.0f);
            PlaySoundMem(se_battle_win, DX_PLAYTYPE_BACK);
            PlaySoundMem(voice_battle_win, DX_PLAYTYPE_BACK);

            // ヒーロー消去フラグ
            for (PhaseTwoHeros* hero_ptr : select_heros)
            {
                if (hero_ptr != nullptr)
                {
                    hero_ptr->is_delete = true;
                }
            }
        }
        else
        {
            // 負け
            ChangeAnimation(eAnimation::eLose, 2.0f);
            PlaySoundMem(se_battle_lose, DX_PLAYTYPE_BACK);
            PlaySoundMem(voice_battle_lose, DX_PLAYTYPE_BACK);
        }

        break;
    case eLose:
        is_game_end = true;

        break;
    case eWin:
        ChangeAnimation(eAnimation::eNone, 0.0f);

        break;
    }
}

void PhaseTwo::ChangeAnimation(eAnimation next, float duration)
{
    now_anime = next;
    anime_second = duration;
}

void PhaseTwo::SetNextWrestler()
{
    // パワーを上げ、上限(45)でクランプ
    wrestler_power += 5;
    if (wrestler_power > 45) wrestler_power = 45;

    // パワー帯からランクとBGMを決める
    int target_bgm = bgn_battle_01;

    if (wrestler_power < 20)
    {
        wrestler_rank = 0;
        target_bgm = bgn_battle_01;
    }
    else if (wrestler_power < 35)
    {
        wrestler_rank = 1;
        target_bgm = bgn_battle_02;
    }
    else
    {
        wrestler_rank = 2;
        target_bgm = bgn_battle_03;
    }

    const int bgms[] = { bgn_battle_01, bgn_battle_02, bgn_battle_03 };

    for (int bgm : bgms)
    {
        // 鳴っていたら止める
        if (bgm != target_bgm && CheckSoundMem(bgm)) {
            StopSoundMem(bgm);
        }
    }

    // ターゲットが鳴っていなければ再生開始
    if (!CheckSoundMem(target_bgm))
    {
        PlaySoundMem(target_bgm, DX_PLAYTYPE_LOOP);
    }

}

void PhaseTwo::SetTotalPower()
{
    int total = 0;
    for (const PhaseTwoHeros* select : select_heros)
    {
        total += select->data.power;
    }
    totalpower = total;
}
