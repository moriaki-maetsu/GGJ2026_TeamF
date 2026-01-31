#pragma once

#include "../SceneBase.h"

class ResultScene : public SceneBase
{
public:
	ResultScene()
		: SceneBase()
	{

	}
	virtual ~ResultScene() = default;
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
		return eSceneType::eResult;
	}
};
