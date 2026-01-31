template <typename T, typename... Args>
T* GameObjectManager::CreateGameObject(const Location2D& new_location, Args&&... args)
{
	static_assert(std::is_base_of<GameObject, T>::value, "T must be derived from GameObject");

	// 引数で渡されたクラスのインスタンスを生成する
	T* new_object = new T(std::forward<Args>(args)...);
	
	// オブジェクトの初期化と位置設定
	new_object->SetLocation(new_location);
	new_object->Initialize();
	
	// 生成リストに追加する
	create_list.emplace_back(new_object);

	// インスタンスのポインタを返却する
	return new_object;
}
