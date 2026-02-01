#pragma once

#include "../Utility/Singleton.h"
#include "HeroData.h"
#include <vector>

/// <summary>
/// “üè‚µ‚½ƒq[ƒ[’B
/// </summary>
class Heros : public Singleton<Heros>
{
private:
	std::vector<HeroData> heros;

public:
	std::vector<HeroData> GetHeros()
	{
		return heros;
	}

	void SetHeros(HeroData data)
	{
		heros.emplace_back(data);
	}
};