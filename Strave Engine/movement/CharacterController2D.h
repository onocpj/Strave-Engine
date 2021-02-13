#pragma once

#include "Keyboard.h"

#include <memory>


#define UNDEFINED_PLAYER      	nullptr
#define REVERSE_DIRECTION	-1.0f

namespace Strave {

    enum class INPUT_AXIS;
    enum class KEY;
    class Player2D;

	class CharacterController2D {

	public:
		CharacterController2D();
		~CharacterController2D();

        /**
            Assings player to character controller

            @param: character - this character will be assigned to character controller
        */
        void AssignCharacter(Player2D& character);
        /**
            Moves character that is assigned to character controller

            @param: axis - axis of movement (horizontal or vertical)
            @param: inputVal - value that represents movement speed of the player
            @param: keyBind - key bind for specific movement
        */
		void Move(INPUT_AXIS axis, float inputVal, Keyboard::KEY keyBind) const;
        void Move(INPUT_AXIS axis, float inputVal) const;

    private:
        Player2D* m_AssignedPlayer2D;

	};

}

