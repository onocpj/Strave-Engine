#pragma once

#define VEC2_ADDITIVE_LIBS

#if defined(VEC2_ADDITIVE_LIBS)
	#include "UDT.h"
#endif // !VEC2_ADDITIVE_LIBS


namespace Strave {

	template<typename Type> class Vector2 {

	public:
		inline Vector2() {}
		inline Vector2(Type x, Type y) : x(x), y(y) {}
		inline virtual ~Vector2() {}

		Type x;
		Type y;

	};

	template<typename Type>
	inline Vector2<Type> operator +=(Vector2<Type>& left, const Vector2<Type>& right) {

		left.x += right.x;
		left.y += right.y;

		return left;

	}
	template<typename Type>
	inline Vector2<Type> operator +(const Vector2<Type>& left, const Vector2<Type>& right) {

		return Vector2<Type>(left.x + right.x, left.y + right.y);

	}
	template<typename Type>
	inline Vector2<Type> operator -=(Vector2<Type>& left, const Vector2<Type>& right) {

		left.x -= right.x;
		left.y -= right.y;

		return left;

	}
	template<typename Type>
	inline Vector2<Type> operator -(const Vector2<Type>& left, const Vector2<Type>& right) {

		return Vector2<Type>(left.x - right.x, left.y - right.y);

	}
	template<typename Type>
	inline Vector2<Type> operator *=(const Vector2<Type>& left, Type right) {

		left.x *= right;
		left.y *= right;

		return left;

	}
	template<typename Type>
	inline Vector2<Type> operator *(const Vector2<Type>& left, Type right) {

		return Vector2<Type>(left.x * right, left.y * right);

	}
	template<typename Type>
	inline Vector2<Type> operator *(Type left, const Vector2<Type>& right) {

		return Vector2<Type>(right.x * left, right.y * left);

	}
	template<typename Type>
	inline Vector2<Type> operator /=(const Vector2<Type>& left, Type right) {

		left.x /= right;
		left.y /= right;

		return left;

	}
	template<typename Type>
	inline Vector2<Type> operator /(const Vector2<Type>& left, Type right) {

		return Vector2<Type>(left.x / right, left.y / right);

	}
	template<typename Type>
	inline Vector2<Type> operator ==(const Vector2<Type>& left, const Vector2<Type>& right) {

		return (left.x == right.x) && (left.y == right.y);

	}
	template<typename Type>
	inline Vector2<Type> operator !=(const Vector2<Type>& left, const Vector2<Type>& right) {

		return (left.x == right.x) || (left.y == right.y);

	}

	// Short cuts for types
	typedef Vector2<float>			Vector2f;
	typedef Vector2<double>			Vector2d;
	typedef Vector2<int>			Vector2i;
	typedef Vector2<unsigned int>	Vector2u;

}


