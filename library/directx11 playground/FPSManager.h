#pragma once

#include <chrono>

namespace ownfos::graphic
{
	// -Basic FPS controller which uses std::chrono::steady_clock(nanoseconds).
	// -Since it is a primitive and simple implementation, real fps might be little bit lower.
	class FPSManager
	{
	private:
		int framesPerSecond;
		long long nanosecondPerFrame;
		long long deltaTime;
		std::chrono::time_point<std::chrono::steady_clock> previousTime;

	public:
		FPSManager(int framesPerSecond);

		// -Returns true if you can proceed to next frame.
		bool IsNextFrame();

		// -Returns time elapsed on processing last frame.
		long long GetDeltaTime();
	};
}