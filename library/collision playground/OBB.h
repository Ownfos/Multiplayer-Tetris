#pragma once

namespace ownfos::collision
{
	class OBB
	{
	private:
		float centerX;
		float centerY;
		float halfWidth;
		float halfHeight;
		float radian;

		// -Returns true if a seperating exists.
		bool PartialSAT(const OBB& other) const;

	public:
		OBB(float centerX, float centerY, float halfWidth, float halfHeight, float radian);
		bool IsColliding(const OBB& other);
	};
}

