#pragma once

#include "SceneType.h"
#include "SceneBase.h"

// シーン生成クラス
class SceneFactory final
{
public:
	static SceneBase* CreateScene(eSceneType new_scene_type);
};
