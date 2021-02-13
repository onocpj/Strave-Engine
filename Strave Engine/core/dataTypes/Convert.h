#pragma once


namespace sf {

	template<typename Type> class Vector2;
	template<typename Type> class Vector3;

}

namespace Strave {

	/*template<typename Type> struct Vector2;
	template<typename Type> struct Vector2v2;*/

	class Convert abstract {

	public:
		template<typename Type> 
			inline static const sf::Vector2<Type> ToVector2(const Strave::Vector2<Type>& vec2) {

				return { vec2.x, vec2.y };
		}
		template<typename Type>
			inline static const Strave::Vector2<Type> ToVector2(const sf::Vector2<Type>& vec2) {

				return { vec2.x, vec2.y };
		}
		template<typename Type>
			inline static const Strave::Vector2f ToVector2f(const sf::Vector2<Type>& vec2) {

				return { (float)vec2.x, (float)vec2.y };
		}
		template<typename Type>
			inline static const Strave::Vector2u ToVector2u(const sf::Vector2<Type>& vec2) {

				return { (unsigned int)vec2.x, (unsigned int)vec2.y };
		}
		template<typename Type>
			inline static const Strave::Vector2i ToVector2i(const sf::Vector2<Type>& vec2) {

				return { (int)vec2.x, (int)vec2.y };
		}
		template<typename Type>
			inline static const Strave::Vector2f ToVector2f(const Strave::Vector2<Type>& vec2) {

			return { (float)vec2.x, (float)vec2.y };
		}
		template<typename Type>
			inline static const Strave::Vector2u ToVector2u(const Strave::Vector2<Type>& vec2) {

			return { (unsigned int)vec2.x, (unsigned int)vec2.y };
		}
		template<typename Type>
			inline static const Strave::Vector2i ToVector2i(const Strave::Vector2<Type>& vec2) {

			return { (int)vec2.x, (int)vec2.y };
		}


	};

}



