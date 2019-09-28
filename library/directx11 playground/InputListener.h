#pragma once

#include "Window.h"

namespace ownfos::graphic
{
	// -Virtual key codes for keyboard and mouse input.
	enum class KeyCode
	{
		// 0 ~ 255 : keyboard (identical with the virtual-key codes)
		Ctrl = VK_CONTROL,
		Enter = VK_RETURN,
		Esc = VK_ESCAPE,
		Space = VK_SPACE,
		BackSpace = VK_BACK,
		Tab = VK_TAB,
		Left = VK_LEFT,
		Up = VK_UP,
		Right = VK_RIGHT,
		Down = VK_DOWN,
		Number0 = 0x30,
		Number1,
		Number2,
		Number3,
		Number4,
		Number5,
		Number6,
		Number7,
		Number8,
		Number9,
		A = 0x41,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,

		// 256 ~ 258 : mouse
		MouseLeft = 256,
		MouseRight,
		MouseMiddle
	};

	// -Frame dependent input information.
	enum class InputState
	{
		// released before and now
		Released,
		// released now but not before
		ReleasingNow,
		// pressed before and now
		Pressing,
		// pressed now but not before
		PressedNow
	};

	// -Virtual class that registers itself to Window and recieves input related window messages.
	// -Inherit this and implement void OnMessage(UINT message, WPARAM wParam, LPARAM lParam).
	class InputListener
	{
	private:
		std::shared_ptr<Window> window;

	public:
		InputListener(std::shared_ptr<Window> window);
		virtual ~InputListener();

		virtual void OnMessage(UINT message, WPARAM wParam, LPARAM lParam) = 0;
	};
}