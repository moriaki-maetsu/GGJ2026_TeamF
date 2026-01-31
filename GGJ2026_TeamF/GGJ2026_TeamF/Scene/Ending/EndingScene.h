#pragma once

#include "../SceneBase.h"

class EndingScene : public SceneBase
{
private:
	class Katzwonus* katzwonus = nullptr;

public:
	EndingScene() : SceneBase() {}
	virtual ~EndingScene() = default;

public:
	virtual void Initialize() override;
	virtual eSceneType Update(float delta_second) override;
	virtual void Draw() const override;
	virtual void Finalize() override;

public:
	virtual eSceneType GetNowSceneType() const override
	{
		return eSceneType::eEnding;
	}

};
