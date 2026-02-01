#include "PhaseTwo.h"
#include "../../Utility/AssetContainer.h"
#include "../../Utility/InputManager.h"
#include "../../Hero/Heros.h"
#include "../Result/ResultData.h"
#include <DxLib.h>

#define LIMIT_LEFT  (0.0f)      // スクロールの左端座標
#define LIMIT_RIGHT (1280.0f)   // スクロールの右端座標

#define CELL_WIDTH  (HERO_SIZE_X * 2.0f + 20.0f)        // 1キャラサイズ+左右の幅
#define TOTAL_LOOP_WIDTH ((heros.size() * CELL_WIDTH>LIMIT_RIGHT)?(heros.size() * CELL_WIDTH):LIMIT_RIGHT)    // 全体のループ幅

#define HERO_IMAGE_SIZE_X   (150)   // 元画像のサイズ
#define HERO_IMAGE_SIZE_Y   (200)   // 元画像のサイズ

void PhaseTwo::Initialize()
{
    // データの取得
    const std::vector<HeroData>& raw_data = Heros::Get()->GetHeros();

    heros.clear();
    heros.reserve(raw_data.size());

    // データを初期化
    for (const HeroData& data : raw_data) {
        heros.push_back({ data, false, false });
    }

    AssetContainer* ac = AssetContainer::Get();

    if (heros.empty())
    {
        int red = ac->GetImages("character_red_02.png")[0];
        int blue = ac->GetImages("character_blue_02.png")[0];
        int green = ac->GetImages("character_green_02.png")[0];
        int pink = ac->GetImages("character_pink_02.png")[0];
        int yellow = ac->GetImages("character_yellow_02.png")[0];

        int i = 1;
        for (int j = 0; j < 4; j++)
        {
            heros.push_back({ { Vector2D{i * CELL_WIDTH + (CELL_WIDTH / 2.0f),100.0f},eColor::eRed,(i++) % 10,red }, false, false });
            heros.push_back({ { Vector2D{i * CELL_WIDTH + (CELL_WIDTH / 2.0f),100.0f},eColor::eBlue,(i++) % 10,blue }, false, false });
            heros.push_back({ { Vector2D{i * CELL_WIDTH + (CELL_WIDTH / 2.0f),100.0f},eColor::eGreen,(i++) % 10,green }, false, false });
            heros.push_back({ { Vector2D{i * CELL_WIDTH + (CELL_WIDTH / 2.0f),100.0f},eColor::ePink,(i++) % 10,pink }, false, false });
            heros.push_back({ { Vector2D{i * CELL_WIDTH + (CELL_WIDTH / 2.0f),100.0f},eColor::eYellow,(i++) % 10,yellow }, false, false });
        }
    }

    for (int i = 0; i < heros.size(); i++)
    {
        heros[i].data.position.y = 80.0f;
    }

    power_badge_image = ac->GetImages("ui_power_badge.png", 10, 10, 1, 120, 100);   // 攻撃力バッジ
    heros_power_ui_image = ac->GetImages("ui_power_plate_player.png")[0];           // 総攻撃力背景UI

    // レスラー
	wrestler_image[0] = AssetContainer::Get()->GetImages("enemy_01.png")[0];
	wrestler_image[1] = AssetContainer::Get()->GetImages("enemy_02.png")[0];
	wrestler_image[2]= AssetContainer::Get()->GetImages("enemy_03.png")[0];
    wrestler_power_ui_image[0] = ac->GetImages("ui_power_plate_enemy_01.png")[0];
    wrestler_power_ui_image[1] = ac->GetImages("ui_power_plate_enemy_02.png")[0];
    wrestler_power_ui_image[2] = ac->GetImages("ui_power_plate_enemy_03.png")[0];
    wrestler_power = 5;
    wrestler_rank = 0;

    // 戦闘ボタン
    start_image[0] = AssetContainer::Get()->GetImages("ui_battle_button_start_01.png")[0];
    start_image[1] = AssetContainer::Get()->GetImages("ui_battle_button_start_02.png")[0];
    start_position = { 640.0f,650.0f };
    start_size = { 150.0f,40.0f };    // 画像サイズの半分の値

    // システム
    background = ac->GetImages("bg_battle_01.png")[0];
    conveyer_image = ac->GetImages("conveyer.png")[0];
    power_number_image = ac->GetImages("ui_number_02.png", 11, 11, 1, 34, 68);

}

eSceneType PhaseTwo::Update(float delta_second)
{
    InputManager* input = InputManager::Get();

    float rot = -input->GetScrollWheel().y * 40.0f;
    scrollx += rot;
    if (scrollx < 0.0f)
        scrollx += TOTAL_LOOP_WIDTH;
    if (scrollx >= TOTAL_LOOP_WIDTH)
        scrollx -= TOTAL_LOOP_WIDTH;

    // 当たり判定の確認処理
    CheckCollision();

    // 死んだヒーローを配列から消去
    heros.erase(
        std::remove_if(heros.begin(), heros.end(),
            [](const PhaseTwoHeros& h) {
                return h.is_delete;
            }
        ),
        heros.end()
    );

    // スクロールキャラの座標を再計算
    for (int i = 0; i < heros.size(); i++)
    {
        heros[i].data.position.x = (i + 1) * CELL_WIDTH + (CELL_WIDTH / 2.0f);
    }

    // ヒーローが残っていなかったら
    if (heros.empty())
    {
        gameend = true;
    }

    if (gameend)
    {
        ResultData::Get()->SetWrestlerCount(wrestler_count);
        return eSceneType::eResult;
    }

    return GetNowSceneType();
}

void PhaseTwo::Draw() const
{
    // 背景の描画
    DrawRotaGraphF(640.0f, 360.0f, 1.f, 0.0f, background, TRUE);
    // ベルトコンベア
    DrawRotaGraphF(640.0f, 80.0f, 0.35f, 0.0f, conveyer_image, TRUE);
    // レスラーの攻撃力背景UI
    DrawRotaGraphF(900.0f, 270.0f, 1.0f, 0.0f, wrestler_power_ui_image[wrestler_rank], TRUE);
    // 合計攻撃力の背景UI
    DrawRotaGraphF(400.0f, 270.0f, 1.0f, 0.0f, heros_power_ui_image, TRUE);
    // 合計攻撃力
    int temp_power = totalpower;
    float draw_x = 480.0f; // 描画の基準位置（1の位の位置）
    float spacing = 16.0f; // 数字同士の間隔

    // 最低でも1回は実行（0の場合でも0を表示）
    do {
        // 1. 一番下の桁を取り出す (例: 123 -> 3)
        int digit = temp_power % 10;

        // 2. その桁の画像を描画
        DrawRotaGraphF(draw_x, 275.0f, 0.5f, 0.0f, power_number_image[digit], TRUE);

        // 3. 次の桁のために描画位置を左にずらし、数値を10で割る
        draw_x -= spacing;
        temp_power /= 10;

    } while (temp_power > 0);

    // レスラーの攻撃力
    temp_power = wrestler_power;
    draw_x = 980.0f; // 描画の基準位置（1の位の位置）
    spacing = 16.0f; // 数字同士の間隔

    // 最低でも1回は実行（0の場合でも0を表示）
    do {
        // 1. 一番下の桁を取り出す (例: 123 -> 3)
        int digit = temp_power % 10;

        // 2. その桁の画像を描画
        DrawRotaGraphF(draw_x, 290.0f, 0.5f, 0.0f, power_number_image[digit], TRUE);

        // 3. 次の桁のために描画位置を左にずらし、数値を10で割る
        draw_x -= spacing;
        temp_power /= 10;

    } while (temp_power > 0);


    DrawLineAA(LIMIT_LEFT, 0.0f, LIMIT_LEFT, HERO_SIZE_Y * 2.0f, 0xFFFFFF, 5.0f);
    DrawLineAA(LIMIT_RIGHT, 0.0f, LIMIT_RIGHT, HERO_SIZE_Y * 2.0f, 0xFFFFFF, 5.0f);

    // ヒーロー選択スクロールの描画
    for (int i = 0; i < heros.size(); ++i) {
        const PhaseTwoHeros& hero = heros[i];

        if (hero.is_selected) continue;
        
        float world_x = hero.data.position.x - scrollx;
            
        // 2. TOTAL_LOOP_WIDTH の範囲内に座標を収める (0 ～ TOTAL_LOOP_WIDTH)
        // これにより、scrollxがどれだけ増減しても、相対位置がループします
        float draw_center_x = fmodf(world_x, TOTAL_LOOP_WIDTH);
        if (draw_center_x < 0) draw_center_x += TOTAL_LOOP_WIDTH;

        // 3. ループの継ぎ目（右端から左端へ）をスムーズに見せるための処理
        // 表示エリアが左端(LIMIT_LEFT)に近い場合、右端から溢れた分を表示させる必要があります
        // 基本的に「画面中央付近」を基準に、遠すぎるものはループ幅分ずらして引き戻します
        if (draw_center_x > LIMIT_RIGHT + HERO_SIZE_X) draw_center_x -= TOTAL_LOOP_WIDTH;
        if (draw_center_x < LIMIT_LEFT - HERO_SIZE_X) draw_center_x += TOTAL_LOOP_WIDTH;

        float left = draw_center_x - HERO_SIZE_X;
        float right = draw_center_x + HERO_SIZE_X;
        float up = hero.data.position.y - HERO_SIZE_Y;
        float down = hero.data.position.y + HERO_SIZE_Y;

        // スクロール外は除外
        if (right<LIMIT_LEFT || left>LIMIT_RIGHT)continue;

        int src_x = 0;
        int src_w = HERO_IMAGE_SIZE_X;

        // 左で見切れていたら
        if (left < LIMIT_LEFT)
        {
            float cut_px = LIMIT_LEFT - left;
            float src_cut = cut_px * (HERO_IMAGE_SIZE_X / (right - left));
            left = LIMIT_LEFT;
            src_x = (int)src_cut;
            src_w -= (int)src_cut;
        }
        // 右で見切れていたら
        if (right > LIMIT_RIGHT)
        {
            float cut_px = right - LIMIT_RIGHT;
            float src_cut = cut_px * (HERO_IMAGE_SIZE_X / (right - left));
            right = LIMIT_RIGHT;
            src_w -= (int)src_cut;
        }

        // 描画
        DrawRectExtendGraphF(
            left, up,
            right, down,
            src_x, 0,
            src_w, HERO_IMAGE_SIZE_Y,
            hero.data.image, TRUE
        );

        // 強さの描画
        Vector2D badgeoffset = { -20.0f,-30.0f };
        //DrawRotaGraphF(right + badgeoffset.x, down + badgeoffset.y, 0.7f, 0.0f, power_badge_image[hero.data.power], TRUE);
        DrawRotaGraphF(world_x - HERO_SIZE_Y + badgeoffset.x, down + badgeoffset.y, 0.7f, 0.0f, power_badge_image[hero.data.power], TRUE);


        // 当たり判定
        DrawBoxAA(left, up, right, down, 0xFFFFFF, FALSE);
        // データの描画
        //DrawFormatStringF(left, down + 5.0f, 0xFFFF00, "Index: %d", i);
        //DrawFormatStringF(left, down + 20.0f, 0xFFFF00, "Power: %d", hero.data.power);
    }

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
        DrawRotaGraphF(position.x, position.y, 1.2f, 0.0f, select->data.image, TRUE);
        Vector2D badgeoffset = { -20.0f,-30.0f };
        DrawRotaGraphF(position.x+HERO_SIZE_X + badgeoffset.x, position.y+HERO_SIZE_Y + badgeoffset.y, 0.84f, 0.0f, power_badge_image[select_heros[i]->data.power], TRUE);
        // 当たり判定
        //DrawBoxAA(position.x - HERO_SIZE_X, position.y - HERO_SIZE_Y, position.x + HERO_SIZE_X, position.y + HERO_SIZE_Y, 0xFFFFFF, FALSE);
    }
    //DrawFormatStringF(150.0f, 380.0f, 0xffffff, "合計パワー:%d", totalpower);

    // レスラーの描画
    DrawRotaGraphF(900.0f, 500.0f, 0.5f, 0.0f, wrestler_image[wrestler_rank], TRUE);
    //DrawFormatStringF(1000.0f, 380.0f, 0xffffff, "レスラーパワー:%d", wrestler_power);

    // 戦闘スタートボタン
    DrawRotaGraphF(start_position.x, start_position.y, 1.f, 0.0f, start_image[is_start_push ? 1 : 0], TRUE);
}

void PhaseTwo::Finalize()
{
}

void PhaseTwo::CheckCollision()
{
    InputManager* input = InputManager::Get();

    // マウスが押された時の処理
    if (input->GetMouseState(MOUSE_INPUT_LEFT) == eInputState::eClick)
    {
        // スクロールキャラクターの判定を行う
        CheckScrollCharacterCollision();
        // 場に出ているキャラクターの判定を行う
        CheckDeselectCollision();

        Vector2D mouse = input->GetMouseLocation();

        // vsが押されたか判定
        if (mouse.x >= start_position.x - start_size.x && mouse.x <= start_position.x + start_size.x &&
            mouse.y >= start_position.y - start_size.y && mouse.y <= start_position.y + start_size.y)
        {
            is_start_push = true;
            if (totalpower >= wrestler_power)
            {
                // 勝ち
                SetNextWrestler();
                wrestler_count++;
                totalpower = 0;
                scrollx = 0.0f;

                // ヒーロー消去フラグ
                for (PhaseTwoHeros* hero_ptr : select_heros)
                {
                    if (hero_ptr != nullptr)
                    {
                        hero_ptr->is_delete = true;
                    }
                }
                select_heros.clear();
            }
            else
            {
                // 負け
                gameend = true;
            }
        }

        // 合計攻撃力を再計算
        SetTotalPower();
    }
}

void PhaseTwo::CheckScrollCharacterCollision()
{
    InputManager* input = InputManager::Get();

    Vector2D mouse = input->GetMouseLocation();

    // スクロールのヒーローが押されたか判定
    for (auto& hero : heros)
    {
        if (hero.is_selected) continue;

        float world_x = hero.data.position.x - scrollx;

        // 2. TOTAL_LOOP_WIDTH の範囲内に座標を収める (0 ～ TOTAL_LOOP_WIDTH)
        // これにより、scrollxがどれだけ増減しても、相対位置がループします
        float draw_center_x = fmodf(world_x, TOTAL_LOOP_WIDTH);
        if (draw_center_x < 0) draw_center_x += TOTAL_LOOP_WIDTH;

        // 3. ループの継ぎ目（右端から左端へ）をスムーズに見せるための処理
        // 表示エリアが左端(LIMIT_LEFT)に近い場合、右端から溢れた分を表示させる必要があります
        // 基本的に「画面中央付近」を基準に、遠すぎるものはループ幅分ずらして引き戻します
        if (draw_center_x > LIMIT_RIGHT + HERO_SIZE_X) draw_center_x -= TOTAL_LOOP_WIDTH;
        if (draw_center_x < LIMIT_LEFT - HERO_SIZE_X) draw_center_x += TOTAL_LOOP_WIDTH;

        float left = draw_center_x - HERO_SIZE_X;
        float right = draw_center_x + HERO_SIZE_X;
        float up = hero.data.position.y - HERO_SIZE_Y;
        float down = hero.data.position.y + HERO_SIZE_Y;

        // スクロール外は除外
        if (right<LIMIT_LEFT || left>LIMIT_RIGHT)continue;

        int src_x = 0;
        int src_w = HERO_IMAGE_SIZE_X;

        // 左で見切れていたら
        if (left < LIMIT_LEFT)
        {
            float cut_px = LIMIT_LEFT - left;
            float src_cut = cut_px * (HERO_IMAGE_SIZE_X / (right - left));
            left = LIMIT_LEFT;
            src_x = (int)src_cut;
            src_w -= (int)src_cut;
        }
        // 右で見切れていたら
        if (right > LIMIT_RIGHT)
        {
            float cut_px = right - LIMIT_RIGHT;
            float src_cut = cut_px * (HERO_IMAGE_SIZE_X / (right - left));
            right = LIMIT_RIGHT;
            src_w -= (int)src_cut;
        }

        // 矢印が重なっていたら
        if (mouse.x >= left && mouse.x <= right &&
            mouse.y >= up && mouse.y <= down)
        {
            // リストの中から「同じ色」のキャラを探し、フラグをfalseに戻す
            for (PhaseTwoHeros* s : select_heros) {
                if (s->data.color == hero.data.color) {
                    s->is_selected = false;
                }
            }

            // リストから「同じ色」の要素を物理的に削除
            select_heros.erase(
                std::remove_if(select_heros.begin(), select_heros.end(),
                    [&](const PhaseTwoHeros* s) {
                        return s->data.color == hero.data.color;
                    }),
                select_heros.end()
            );

            // 新しく追加する
            select_heros.push_back(&hero);
            hero.is_selected = true;
        }
    }
}

void PhaseTwo::CheckDeselectCollision()
{
    InputManager* input = InputManager::Get();

    Vector2D mouse = input->GetMouseLocation();

    // 戦闘するヒーローが押されたか判定
    int i = 0; // カウンターを用意
    select_heros.erase(
        std::remove_if(select_heros.begin(), select_heros.end(), [&](PhaseTwoHeros* hero) {
            // 1. このヒーローが「場」で表示されるべき位置を計算
            // i 番目のヒーローとして位置を特定する

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

            // 判定が終わる前にカウンターを進める
            i++;

            // 2. 計算した position を使って当たり判定
            Vector2D lu = { position.x - HERO_SIZE_X, position.y - HERO_SIZE_Y };
            Vector2D rd = { position.x + HERO_SIZE_X, position.y + HERO_SIZE_Y };

            // 3. クリックされたか判定
            if (mouse.x >= lu.x && mouse.x <= rd.x && mouse.y >= lu.y && mouse.y <= rd.y)
            {
                // セレクト前にフラグを戻す
                hero->is_selected = false;
                return true; // 削除
            }
            return false; // 保持
            }),
        select_heros.end()
    );
}

void PhaseTwo::SetNextWrestler()
{
    // 攻撃力
    wrestler_power += 10;
    wrestler_rank = 0;
    if (wrestler_power > 20)
    {
        wrestler_rank = 1;
        if (wrestler_power > 35)
        {
            wrestler_rank = 2;
        }
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
