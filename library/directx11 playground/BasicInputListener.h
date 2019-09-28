#pragma once

#include <tuple>
#include <list>

#include "InputListener.h"

namespace ownfos::graphic
{
	// -Handles mouse/keyboard input by reading messages from window.
	// -Provides text buffer which can be used to implement text input.
	//
	// -----Example usage-----
	// BasicInputListener input(window);
	// input.UpdateFrameState();
	// auto state = input.FrameState(KeyCode::Esc);
	class BasicInputListener : public InputListener
	{
	private:
		std::shared_ptr<Window> window;

		int mouseX = 0;
		int mouseY = 0;
		// -Raw state tells us whether a key is currently pressed or not.
		// -Quite primitive, huh?
		bool rawState[256 + 3] = {};
		// -In contrast to raw state, frame state also gives us information about previous state.
		// -If you want to know "when" a key is pressed or released, you'll have to use this.
		// -Just don't forget to call UpdateFrameState() whenever new frame starts.
		InputState frameState[256 + 3] = {};

	public:
		BasicInputListener(std::shared_ptr<Window> window);

		// -Updates raw input state.
		void OnMessage(UINT message, WPARAM wParam, LPARAM lParam) override;

		// -Calculates this frame's input state.
		// -Call this function at the start of each frame
		//  if you wish to use FrameState().
		void UpdateFrameState();

		// -Returns raw input state (true if pressed).
		// -If you want to know whether this key is pressed/released
		//  on current frame, use FrameState() instead.
		bool RawState(KeyCode keyCode);

		// -Returns this frame's input state.
		InputState FrameState(KeyCode keyCode);

		// -Returns x coordinate of mouse in normalized coordinate(-1 ~ 1).
		float MouseX();

		// -Returns y coordinate of mouse in normalized coordinate(-1 ~ 1).
		float MouseY();
	};
}