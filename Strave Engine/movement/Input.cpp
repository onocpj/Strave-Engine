#include "Input.h"

namespace Strave {

	Input::Input() {

	}
	Input::~Input() {

	}

	float Input::GetAxisDirection(INPUT_DIRECTION axis) const {

		switch (axis) {

		case Strave::INPUT_DIRECTION::UP:
		case Strave::INPUT_DIRECTION::LEFT:
			return MOVE_VAL * MOVE_INVERT;
			break;

		case Strave::INPUT_DIRECTION::DOWN:
		case Strave::INPUT_DIRECTION::RIGHT:		
			return MOVE_VAL;
			break;

		default:
			return MOVE_VAL;
			break;
		}

	}

}

