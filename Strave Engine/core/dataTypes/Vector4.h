#pragma once


#define VEC2V2_ADDITIONAL_LIBS 

#if defined(VEC2V2_ADDITIONAL_LIBS)
	#include "Vector2.h"
#endif


namespace Strave {

	template<typename Type> class Vector2;

	template<typename Type> class Vector4 {

	public:
		inline Vector4() {}
		inline Vector4(Vector2<Type> min, Vector2<Type> max) : min(min), max(max) {}
		inline virtual ~Vector4() {}

		Vector2<Type> min;
		Vector2<Type> max;

	};

	template<typename Type>
	inline Vector4<Type> operator +=(Vector4<Type>& left, const Vector4<Type>& right) {

		left.min.x += right.min.x;
		left.min.y += right.min.y;
		left.max.x += right.max.x;
		left.max.y += right.max.y;

		return left;

	}
	template<typename Type>
	inline Vector4<Type> operator +(const Vector4<Type>& left, const Vector4<Type>& right) {

		return Vector4<Type>(
			Vector2<Type>(left.min.x + right.min.x, left.min.y + right.min.y),
			Vector2<Type>(left.max.x + right.max.x, left.max.y + right.max.y)
			);

	}
	template<typename Type>
	inline Vector4<Type> operator -=(Vector4<Type>& left, const Vector4<Type>& right) {

		left.min.x -= right.min.x;
		left.min.y -= right.min.y;
		left.max.x -= right.max.x;
		left.max.y -= right.max.y;

		return left;

	}
	template<typename Type>
	inline Vector4<Type> operator -(const Vector4<Type>& left, const Vector4<Type>& right) {

		return Vector4<Type>(
			Vector2<Type>(left.min.x - right.min.x, left.min.y - right.min.y),
			Vector2<Type>(left.max.x - right.max.x, left.max.y - right.max.y)
			);

	}
	template<typename Type>
	inline Vector4<Type> operator *=(const Vector4<Type>& left, Type right) {

		left.min.x *= right;
		left.min.y *= right;
		left.max.x *= right;
		left.max.y *= right;

		return left;

	}
	template<typename Type>
	inline Vector4<Type> operator *(const Vector4<Type>& left, Type right) {

		return Vector4<Type>(
			Vector2<Type>(left.min.x * right, left.min.y * right),
			Vector2<Type>(left.max.x * right, left.max.y * right)
			);

	}
	template<typename Type>
	inline Vector4<Type> operator *(Type left, const Vector4<Type>& right) {

		return Vector4<Type>(
			Vector2<Type>(left * right.min.x, left * right.min.y),
			Vector2<Type>(left * right.max.x, left * right.max.y)
			);

	}
	template<typename Type>
	inline Vector4<Type> operator /=(const Vector4<Type>& left, Type right) {

		left.min.x /= right;
		left.min.y /= right;
		left.max.x /= right;
		left.max.y /= right;

		return left;

	}
	template<typename Type>
	inline Vector4<Type> operator /(const Vector4<Type>& left, Type right) {

		return Vector4<Type>(
			Vector2<Type>(left.min.x / right, left.min.y / right),
			Vector2<Type>(left.max.x / right, left.max.y / right)
			);

	}
	template<typename Type>
	inline Vector4<Type> operator ==(const Vector4<Type>& left, const Vector4<Type>& right) {

		return ((left.min.x == right.min.x) && (left.min.y == right.min.y)) &&
			((left.max.x == right.max.x) && (left.max.y == right.max.y));

	}
	template<typename Type>
	inline Vector4<Type> operator !=(const Vector4<Type>& left, const Vector4<Type>& right) {

		return ((left.min.x == right.min.x) && (left.min.y == right.min.y)) ||
			((left.max.x == right.max.x) && (left.max.y == right.max.y));

	}

	// Short cuts for types
	typedef Vector4<float>		Vector4f;
	typedef Vector4<double>		Vector4d;
	typedef Vector4<int>		Vector4i;
	typedef Vector4<unsigned int>	Vector4u;

}
