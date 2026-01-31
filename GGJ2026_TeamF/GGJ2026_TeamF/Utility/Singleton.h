#pragma once

#include <mutex>

/// <summary>
/// Singletonパターン基底クラス
/// </summary>
/// <typeparam name="T">シングルトンにしたいクラス名</typeparam>
template <class T>
class Singleton
{
protected:
	Singleton() = default;
	~Singleton() = default;
	Singleton(const Singleton&) = delete;
	Singleton& operator = (const Singleton&) = delete;

public:
	static T* Get()
	{
		static std::once_flag init_flag;
		static T* instance = nullptr;
		std::call_once(init_flag, []() {
			instance = new T();
			std::atexit(Destroy);
		});
		return instance;
	}

private:
	static void Destroy()
	{
		delete Get();
	}
};
