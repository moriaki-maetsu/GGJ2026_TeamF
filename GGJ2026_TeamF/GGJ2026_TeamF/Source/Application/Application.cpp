#include "../resource.h"
#include "Application.h"
#include "../Utility/InputManager.h"
#include "../Utility/AssetContainer.h"
#include "../Scene/SceneManager.h"
#include "ProjectConfig.h"

#include "DxLib.h"

bool Application::WakeUp()
{
	// アイコンの設定
	SetWindowIconID(IDI_MYICON);

	// Log.txtを出力しないようにする
	SetOutApplicationLogValidFlag(FALSE);

	// ウィンドウモードの設定（Debugモード:ウィンドウ, Releaseモード:フルスクリーン）
#if _DEBUG
	ChangeWindowMode(TRUE);
#else
	ChangeWindowMode(TRUE);
#endif // _DEBUG

	// 画面サイズの設定
	SetGraphMode(D_WINDOW_SIZE_X, D_WINDOW_SIZE_Y, 32);

	// ウィンドウテキストの設定
	SetWindowText("GGJ2026_TeamF");

	// ＤＸライブラリの初期化
	if (DxLib_Init() == -1)
	{
		// 起動に失敗したことを通知する
		return false;
	}

	// 裏画面から描画を始める（ダブルバッファ）
	SetDrawScreen(DX_SCREEN_BACK);

	// バックグラウンド状態でも動作させる
	SetAlwaysRunFlag(TRUE);

	// BMP形式の透過設定
	SetTransColor(0, 100, 0);

	// 起動に成功したことを通知する
	return true;
}

void Application::Run()
{
	try
	{
		// シーンマネージャー機能の取得
		SceneManager* scene_manager = SceneManager::Get();

		// シーンマネージャーの初期化
		scene_manager->Initialize();

		// メインループ
		while (ProcessMessage() == 0 && execute_flag)
		{
			// 1フレーム当たりの時間を計測する
			DeltaSecondMeasure();

			if (delta_second >= (1.0f / 60.0f))
			{
				// 入力情報の更新
				InputManager* input = InputManager::Get();
				input->Update();

				// シーンの更新と描画
				execute_flag = scene_manager->Update(delta_second);

				while (delta_second >= (1.0f / 60.0f))
				{
					delta_second -= 1.0f / 60.0f;
				}
			}

			// 終了確認
			CheckExecute();
		}
	}
	catch (const std::exception& except)
	{
		// 例外キャッチで内容を出力する
		OutputDebugString(except.what());
	}
}

void Application::Close()
{
	// 保持している内容を破棄する
	SceneManager::Get()->Finalize();
	AssetContainer::Get()->AllRelease();
	GameObjectManager::Get()->Release();

	// ＤＸライブラリの使用を終了する
	DxLib_End();
}

void Application::DeltaSecondMeasure()
{
	LONGLONG now = GetNowHiPerformanceCount();
	
	// PCが起動してからの経過時間を取得
	static LONGLONG	elapsed_time = GetNowHiPerformanceCount();	

	// １フレーム当たりの経過時間（μ秒）を計算
	LONGLONG took = now - elapsed_time;

	// μ秒から秒に分解能を変換する
	delta_second += static_cast<float>(took) * 1.0e-6f;
	
	// PCが起動してからの経過時間を更新する
	elapsed_time = now;

	

}

void Application::CheckExecute()
{
	// 入力情報の取得
	InputManager* input = InputManager::Get();

	//　強制終了ボタンが押された
	if ((input->GetKeyState(KEY_INPUT_ESCAPE) == eInputState::eRelease) ||
		(input->GetButtonState(XINPUT_BUTTON_BACK) == eInputState::eRelease))
	{
		execute_flag = false;
	}
}
