#include "DxLib.h"
#include "PhaseOne.h"
#include "../../Utility/AssetContainer.h"

void PhaseOne::Initialize()
{
	AssetContainer* container = AssetContainer::Get();
	hero[0].color = Color::eRed;
	hero[0].image = container->GetImages("")
}

eSceneType PhaseOne::Update(float delta_second)
{
	
	return GetNowSceneType();
}

void PhaseOne::Draw() const
{
	
}

void PhaseOne::Finalize()
{
}
