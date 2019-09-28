#pragma once

#include <tuple>

namespace ownfos::collision
{
	// -Returns coordinate of point (x, y) rotated with specified angle.
	std::tuple<float, float> RotatedPoint(float x, float y, float radian);
}
