#pragma once

#include "../Utility/Singleton.h"

class Application final : public Singleton<Application>
{
private:
	// 1フレーム当たりの時間
	float delta_second = 0.0f;
	// ループ実行フラグ
	bool execute_flag = true;

public:
	// 起動確認
	bool WakeUp();
	// 実行処理
	void Run();
	// 閉じる
	void Close();

private:
	// 1フレーム当たりの時間を計測する
	void DeltaSecondMeasure();
	// 続行確認処理
	void CheckExecute();

};
