#pragma once

namespace ownfos::collision
{
	class AABB
	{
	private:
		float left;
		float bottom;
		float right;
		float top;

	public:
		AABB(float centerX, float centerY, float halfWidth, float halfHeight);
		bool IsColliding(const AABB& other);
		bool IsPointInside(float x, float y);
	};
}

