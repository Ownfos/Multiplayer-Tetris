#pragma once

#include "InputListener.h"

namespace ownfos::graphic
{
	// -Listens to mouse wheel event.
	// -Inherit this and Implement void OnWheelUp() and void OnWheelDown().
	class MouseWheelListener : public InputListener
	{
	public:
		MouseWheelListener(std::shared_ptr<Window> window);

		void OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
		virtual void OnWheelUp() = 0;
		virtual void OnWheelDown() = 0;
	};
}

