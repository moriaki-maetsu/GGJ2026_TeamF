#include "SceneFactory.h"

#include "Title/TitleScene.h"
#include "InGame/InGameScene.h"
#include "Result/ResultScene.h"
#include "Ending/EndingScene.h"

SceneBase* SceneFactory::CreateScene(eSceneType new_scene_type)
{
	// ˆø”‚É‚æ‚Á‚Ä¶¬‚·‚éƒV[ƒ“‚ğØ‚è‘Ö‚¦‚é
	switch (new_scene_type)
	{
	
		case eSceneType::eTitle:
			return dynamic_cast<SceneBase*>(new TitleScene());
		case eSceneType::eInGame:
			return dynamic_cast<SceneBase*>(new InGameScene());
		case eSceneType::eResult:
			return dynamic_cast<SceneBase*>(new ResultScene());

		case eSceneType::eEnding:
			return dynamic_cast<SceneBase*>(new EndingScene());
		case eSceneType::eExit:
		default:
			return nullptr;
	}
}
