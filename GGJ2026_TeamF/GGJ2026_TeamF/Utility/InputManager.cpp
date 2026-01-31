#include "InputManager.h"
#include <mutex>
#include "DxLib.h"

void InputManager::Update()
{
	/////////////////////////////////////
	// キーボード入力値の更新
	/////////////////////////////////////
	char buf[KEYCODE_MAX] = {};
	GetHitKeyStateAll(buf);
	for (size_t i = 0ULL; i < KEYCODE_MAX; i++)
	{
		bool flag = static_cast<bool>(buf[i] == TRUE);
		eInputState next_state = NextInputState(key[i], flag);
		key[i] = next_state;
	}

	
	/////////////////////////////////////
	// コントローラー入力値の更新
	/////////////////////////////////////
	XINPUT_STATE controller = {};
	GetJoypadXInputState(DX_INPUT_PAD1, &controller);
	for (size_t i = 0ULL; i < BUTTON_MAX; i++)
	{
		bool flag = static_cast<bool>(controller.Buttons[i] == TRUE);
		eInputState next_state = NextInputState(button[i], flag);
		button[i] = next_state;
	}

	trigger[0] = TriggerNormalize(controller.LeftTrigger);
	trigger[1] = TriggerNormalize(controller.RightTrigger);

	stick[0].x = StickNormalize(controller.ThumbLX);
	stick[0].y = StickNormalize(controller.ThumbLY);
	stick[1].x = StickNormalize(controller.ThumbRX);
	stick[1].y = StickNormalize(controller.ThumbRY);


	/////////////////////////////////////
	// マウス入力値の更新
	/////////////////////////////////////
	int mx, my;
	GetMousePoint(&mx, &my);
	mouse_location = Location2D(static_cast<float>(mx), static_cast<float>(my));

	mouse_wheel.x = GetMouseHWheelRotVolF();
	mouse_wheel.y = GetMouseWheelRotVolF();

	old_mouse_button = now_mouse_button;
	now_mouse_button = GetMouseInput();
}

eInputState InputManager::GetKeyState(size_t keycode) const
{
	if (keycode >= KEYCODE_MAX)
	{
		throw std::out_of_range("Keycode is out of range");
	}
	return this->key[keycode];
}

eInputState InputManager::GetButtonState(size_t button) const
{
	if (button >= KEYCODE_MAX)
	{
		throw std::out_of_range("Button is out of range");
	}
	return this->button[button];
}

eInputState InputManager::GetMouseState(int button) const
{
	if (old_mouse_button & button)
	{
		if (now_mouse_button & button)
		{
			return eInputState::ePressed;
		}
		else
		{
			return eInputState::eRelease;
		}
	}
	else
	{
		if (now_mouse_button & button)
		{
			return eInputState::eClick;
		}
	}
	return eInputState::eNone;
}

const Vector2D& InputManager::GetLeftStick() const
{
	return stick[0];
}

const Vector2D& InputManager::GetRightStick() const
{
	return stick[1];
}

float InputManager::GetLeftTrigger() const
{
	return trigger[0];
}

float InputManager::GetRightTrigger() const
{
	return trigger[1];
}

const Location2D& InputManager::GetMouseLocation() const
{
	return mouse_location;
}

const Vector2D& InputManager::GetScrollWheel() const
{
	return mouse_wheel;
}

eInputState InputManager::NextInputState(eInputState now_state, bool flag) const
{
	if (flag)
	{
		if (now_state == eInputState::eNone || now_state == eInputState::eRelease)
		{
			return eInputState::eClick;
		}
		return eInputState::ePressed;
	}
	else
	{
		if (now_state == eInputState::ePressed || now_state == eInputState::eClick)
		{
			return eInputState::eRelease;
		}
		return eInputState::eNone;
	}	
}

float InputManager::TriggerNormalize(unsigned char value) const
{
	float fv = static_cast<float>(value);
	return (fv / static_cast<float>(UCHAR_MAX));
}

float InputManager::StickNormalize(short value) const
{
	float fv = static_cast<float>(value);
	if (fv >= 0.0f)
	{
		return fv / static_cast<float>(SHRT_MAX);
	}
	else
	{
		return -(fv / static_cast<float>(SHRT_MIN));
	}
}

