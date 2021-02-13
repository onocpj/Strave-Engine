#include "Math.h"

namespace Strave {

	float Math::Round(float val, unsigned int floatingPoint) {

		int t_FloatingValue = 1;
		float t_Result = val;

		if (floatingPoint == 0) {
			t_Result = (float)((int)val);

		} else {
			for (unsigned int i = 0; i != floatingPoint; i++) {
				t_FloatingValue *= 10;
			}

			if (val >= 0) {
				t_Result = (float)((int)(t_Result * t_FloatingValue + 0.5f));

			} else {
				t_Result = (float)((int)(t_Result * t_FloatingValue - 0.5f));
			}
		}

		return t_Result / t_FloatingValue;

	}
	double Math::Round(double val, unsigned int floatingPoint) {

		int t_FloatingValue = 1;
		double t_Result = val;

		if (floatingPoint == 0) {
			t_Result = (double)((int)val);

		}
		else {
			for (unsigned int i = 0; i != floatingPoint; i++) {
				t_FloatingValue *= 10;
			}

			if (val >= 0) {
				t_Result = (float)((int)(t_Result * t_FloatingValue + 0.5));

			} else {
				t_Result = (float)((int)(t_Result * t_FloatingValue - 0.5));
			}
		}

		return t_Result / t_FloatingValue;

	}
	float Math::DegreesToRadians(float degrees) {

		return degrees * MATH_PI / 180.0f;

	}
	double Math::DegreesToRadians(double degrees) {

		return degrees * MATH_PI / 180.0;

	}

}
