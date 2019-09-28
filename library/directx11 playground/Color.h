#pragma once

namespace ownfos::graphic
{
	// -Stores color information.
	// -Each color component has value of 0.0f ~ 1.0f.
	// -Every other value(higher or lower than given range) will be clamped automatically.
	// -Supports linear interpolation as static function.
	//
	// -----Example usage-----
	// Color red(0xff0000ff);
	// Color green(0U, 255U, 0U, 255U);
	// Color blue(0.0f, 0.0f, 1.0f, 1.0f);
	// Color mixed = Color::Interpolate(red, green, 0.5f);
	class Color
	{
	private:
		float red;
		float green;
		float blue;
		float alpha;

	public:
		Color(const Color& other);

		// -Initialize with normalized value (0 ~ 1).
		Color(float red, float green, float blue, float alpha);

		// -Initialize with 8 bit value (0 ~ 255).
		Color(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha);

		// -Initialize with 8 digit hexadecimal value.
		Color(unsigned int hexCode);

		Color& operator=(const Color& other);

		float Red();
		float Green();
		float Blue();
		float Alpha();

		// -Returns linear interpolation of colors.
		static Color Interpolate(Color left, Color right, float ratio);
	};
}



