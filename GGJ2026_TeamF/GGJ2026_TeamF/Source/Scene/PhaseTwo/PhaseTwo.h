#pragma once

#include "../SceneBase.h"
#include "../../Hero/Heros.h"

class PhaseTwo : public SceneBase
{
private:

	struct PhaseTwoHeros
	{
		HeroData data;
		bool is_selected;
	};

	std::vector<PhaseTwoHeros> heros;

	float scrollx;

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
