#include "Input.h"

namespace Strave {

	Input::Input() {

	}
	Input::~Input() {

	}

	float Input::GetAxisDirection(InputDirection axis) const {

		switch (axis) {

		case Strave::InputDirection::Up:
		case Strave::InputDirection::Left:

			return MOVE_VAL * MOVE_INVERT;

			break;

		case Strave::InputDirection::Down:
		case Strave::InputDirection::Right:

			return MOVE_VAL;

			break;

		default:

			return MOVE_VAL;

			break;
		}

	}

}

