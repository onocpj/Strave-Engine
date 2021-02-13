#pragma once


#define MOVE_VAL	100.0f
#define MOVE_INVERT	-1.0f


namespace Strave {

	enum class INPUT_DIRECTION {
		UNDEFINED	= -1,
		UP,
		DOWN,
		LEFT,
		RIGHT
	};
	enum class INPUT_AXIS {
		UNDEFINED	= -1,
		HORIZONTAL,
		VERTICAL
	};

	class Input {

	public:
		Input();
		~Input();

		/// \brief Gets axis direction movment value
		///
		/// \param direction - Direction to move
		/// 
		/// \return Direction movement value
		float GetAxisDirection(INPUT_DIRECTION direction) const;

	};

}

