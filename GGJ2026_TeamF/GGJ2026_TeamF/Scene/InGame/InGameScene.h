#pragma once

#include "../SceneBase.h"

class InGameScene : public SceneBase
{
public:
	InGameScene()
		: SceneBase()
	{

	}
	virtual ~InGameScene() = default;
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
		return eSceneType::eInGame;
	}
};
