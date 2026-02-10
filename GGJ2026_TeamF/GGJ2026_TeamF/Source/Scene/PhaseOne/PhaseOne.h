#pragma once

#include "../SceneBase.h"
#include "../../Hero/HeroData.h"
#include "../../Hero/Belt.h"

class PhaseOne : public SceneBase
{
private:
	HeroData hero[5];
	Belt belt[5];
	Belt belt_icon[5];
	float display_time_count;
	float display_time;
	float timelimit_count;
	bool start_flag;
	float start_count;
	int conveyer_x;
	int bgm;
	int belt_click_se;
	int change_se;
	int miss_se;

public:
	PhaseOne()
		: SceneBase()
		, hero()
		, belt()
		, belt_icon()
		, display_time_count()
		, display_time()
		, timelimit_count()
		, start_flag()
		, start_count()
		, conveyer_x()
		, bgm()
		, belt_click_se()
		, change_se()
		, miss_se()
	{

	}
	virtual ~PhaseOne() = default;
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
		return eSceneType::ePhaseOne;
	}
};
