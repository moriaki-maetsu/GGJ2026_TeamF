#pragma once
#pragma once

#include "../SceneBase.h"

class PhaseTwo : public SceneBase
{
public:
	PhaseTwo()
		: SceneBase()
	{

	}
	virtual ~PhaseTwo() = default;
public:
	// ‰Šú‰»
	virtual void Initialize() override;
	// XV
	virtual eSceneType Update(float delta_second) override;
	// •`‰æ
	virtual void Draw() const override;
	// I—¹
	virtual void Finalize() override;

public:
	virtual eSceneType GetNowSceneType() const override
	{
		return eSceneType::ePhaseTwo;
	}
};
