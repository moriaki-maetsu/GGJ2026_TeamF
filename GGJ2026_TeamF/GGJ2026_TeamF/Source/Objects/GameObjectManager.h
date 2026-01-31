#pragma once

#include "../Utility/Singleton.h"
#include "../Utility/Geometry.h"
#include <list>
#include <vector>

#define D_FIXED_TIME (1.0f / 60.0f)

class GameObject;

class GameObjectManager final : public Singleton<GameObjectManager>
{
private:
	std::list<GameObject*> create_list;
	std::list<GameObject*> destroy_list;
	std::vector<GameObject*> game_object_list;

	float fixed_time = 0.0f;
	bool updating_objects = true;

public:
	void Update(float delta_second);
	void Draw(const Location2D& screen_offset) const;

public:
	template <typename T, typename... Args>
	T* CreateGameObject(const Location2D& new_location, Args&&... args);

	void DestroyGameObject(GameObject* target);

public:
	void Release();

private:
	void CheckCreateGameObject();
	void CheckDestroyGameObject();
	void CheckHitCollision(GameObject* obj1, GameObject* obj2);

};

#include "GameObjectManager.inl"
