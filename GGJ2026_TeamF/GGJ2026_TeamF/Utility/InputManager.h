#pragma once

#include "Singleton.h"
#include "Geometry.h"
#include <cstdint>

// 入力状態列挙クラス
enum class eInputState : uint8_t
{
	eNone,		// 未入力
	eClick,		// 押した瞬間
	eRelease,	// 離した瞬間
	ePressed,	// 押している間
};

/// <summary>
/// 入力管理クラス
/// </summary>
class InputManager final : public Singleton<InputManager>
{
private:
	static const size_t KEYCODE_MAX = 256;	// キー最大数
	static const size_t BUTTON_MAX = 16;	// ボタン最大数

private:
	eInputState key[KEYCODE_MAX] = {};

	eInputState button[KEYCODE_MAX] = {};
	Vector2D stick[2] = {};
	float trigger[2] = {};

	int old_mouse_button = 0;
	int now_mouse_button = 0;
	Location2D mouse_location = 0.0f;
	Vector2D mouse_wheel = 0.0f;

public:
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

public:
	eInputState GetKeyState(size_t keycode) const;
	eInputState GetButtonState(size_t button) const;
	eInputState GetMouseState(int button) const;
	const Vector2D& GetLeftStick() const;
	const Vector2D& GetRightStick() const;
	float GetLeftTrigger() const;
	float GetRightTrigger() const;
	const Location2D& GetMouseLocation() const;
	const Vector2D& GetScrollWheel() const;

private:
	eInputState NextInputState(eInputState now_state, bool flag) const;
	float TriggerNormalize(unsigned char value) const;
	float StickNormalize(short value) const;

};
