#pragma once

#include "../SceneBase.h"
#include "../../Hero/HeroData.h"
#include "../../Hero/Belt.h"

class PhaseOne : public SceneBase
{
private:
	int time;
	HeroData hero[5];
	Belt belt[5];
	int display_count;
public:
	PhaseOne()
		: SceneBase()
		, time()
		, hero()
		, belt()
		, display_count()
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
