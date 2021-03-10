#pragma once

#include "../Vector2.h"

namespace Strave {

	namespace Shapes {

		template<typename Type> class SV_SANDBOX_API Point : public Vector2<Type> {
		public:
			template<typename Type>
			static inline Type DistanceBetweenPoints(const Vector2<Type>& p1, const Vector2<Type>& p2) 
			{
				return (float)sqrt(
					pow((float)(p1.x - p2.x), 2) +
					pow((float)(p1.y - p2.y), 2)
				);
			}
			template<typename Type>
			static inline bool IsInRange(Type val1, Type val2) 
			{
				return val1 <= val2 ? true : false;
			}
		};
		class SV_SANDBOX_API Circle {
		public:
			Circle();
			Circle(const Vector2f& position, float radius);
			virtual ~Circle();

			inline void SetPosition(const Vector2f& position) { m_Position = position; }
			inline void SetRadius(float radius) { m_Radius = radius; }

			inline const Vector2f& GetPostion(void) const { return m_Position; }
			inline const float& GetRadius(void) const { return m_Radius; }

			template<typename Type>
			static inline bool InternalCircleBoundingTest(const Vector2<Type>& c1Point, const Vector2<Type>& c2Point, const float& c1Rad, const float& c2Rad) 
			{
				if (Shapes::Point<float>::DistanceBetweenPoints<float>(c1Point, c2Point) >= (c2Rad - c1Rad)) {
					return true;

				} else {
					return false;
				}
			}

		private:
			Vector2f	m_Position;
			float		m_Radius;

		};

		typedef Point<float>		fPoint;
		typedef Point<unsigned int>	uPoint;
		typedef Point<int>			iPoint;
	};

};

