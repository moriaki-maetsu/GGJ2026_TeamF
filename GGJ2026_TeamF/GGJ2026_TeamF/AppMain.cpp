#include "DxLib.h"
#include "Application/Application.h"

// メイン関数
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	Application app;

	// アプリケーションの起動確認
	if (app.WakeUp())
	{
		// 実行処理
		app.Run();
		// 終了処理
		app.Close();
	}

	// 処理の終了
	return 0;
}
