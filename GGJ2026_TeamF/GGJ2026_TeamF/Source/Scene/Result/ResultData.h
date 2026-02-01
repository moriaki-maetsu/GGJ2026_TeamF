#pragma once
#include "../../Utility/Singleton.h"

class ResultData : public Singleton<ResultData>
{
private:
	int wrestler_count;		// ƒŒƒXƒ‰[‚ğ“|‚µ‚½”

public:
	void SetWrestlerCount(int count)
	{
		wrestler_count = count;
	}

	int GetWrestlerCount()
	{
		return wrestler_count;
	}
};