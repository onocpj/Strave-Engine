#pragma once

#include "../core/Core.h"


#define MOVE_VAL			100.0f
#define MOVE_INVERT			-1.0f


namespace Strave {

	enum class InputDirection 
	{
		Undefined	= -1,
		Up,
		Down,
		Left,
		Right
	};
	enum class InputAxis 
	{
		Undefined	= -1,
		Horizontal,
		Vertical
	};

	class SV_SANDBOX_API Input 
	{
	public:
		Input();
		~Input();

		/// \brief Gets axis direction movment value
		///
		/// \param direction - Direction to move
		/// 
		/// \return Direction movement value
		float GetAxisDirection(InputDirection direction) const;

	};

}

