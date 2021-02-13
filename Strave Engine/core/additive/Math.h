#pragma once

#define MATH_PI 3.14159265f

namespace Strave {

	class Math abstract {
	public:
		static float Round(float val, unsigned int floatingPoint = 1);
		static double Round(double val, unsigned int floatingPoint = 1);
		static float DegreesToRadians(float degrees);
		static double DegreesToRadians(double degrees);
	}; 

}

