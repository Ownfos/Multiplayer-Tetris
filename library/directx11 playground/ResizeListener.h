#pragma once

#include "Window.h"

namespace ownfos::graphic
{
	// -Virtual class that observes window's resize event.
	// -Inherit this class and it will automatically register to window.
	//
	// -----ExampleUsage-----
	// class SizeDependentSomething : public ResizeListener
	// {
	//      SizeDependentSomething(Window& window) : ResizeListener(window) {}
	//      void OnResize(int width, int height) override {}
	// }
	class ResizeListener
	{
	private:
		std::shared_ptr<Window> window;
	public:
		ResizeListener(std::shared_ptr<Window> window);
		virtual ~ResizeListener();
		virtual void OnResize(int width, int height) = 0;
	};
}