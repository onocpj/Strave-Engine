#pragma once

#include "../core/Core.h"
#include "../core/datatypes/UDT.h"

#define UNDEFINED_KEYBOARD_KEY       ERROR_TYPE

#if defined(USE_SFML_INPUT)
    namespace sf { class SV_SANDBOX_API Keyboard; }
#endif

namespace Strave {

	class SV_SANDBOX_API Keyboard 
    {
	public:
        enum class KEY {

            UNDEFINED = -1, A = 0, B, C,
            D, E, F, G,
            H, I, J, K,
            L, M, N, O,
            P, Q, R, S,
            T, U, V, W,
            X, Y, Z, Num0,
            NUM_1, NUM_2, NUM_3, NUM_4,
            NUM_5, NUM_6, NUM_7, NUM_8,
            NUM_9, ESCAPE, L_CONTROL, L_SHIFT,
            L_ALT, L_SYSTEM, R_CONTROL, R_SHIFT,
            R_ALT, R_SYSTEM, MENU, L_BRACKET,
            R_BRACKET, SEMICOLON, COMMA, PERIOD,
            QUOTE, SLASH, BACKSLASH, TILDE,
            EQUAL, HYPEN, SPACE, ENTER,
            BACKSPACE, TAB, PAGE_UP, PAGE_DOWN,
            END, HOME, INSERT, DELETE,
            ADD, SUBSTRACT, MULTIPLY, DIVIDE,
            LEFT, RIGHT, UP, DOWN,
            NUMPAD_0, NUMPAD_1, NUMPAD_2, NUMPAD_3,
            NUMPAD_4, NUMPAD_5, NUMPAD_6, NUMPAD_7,
            NUMPAD_8, NUMPAD_9, F1, F2,
            F3, F4, F5, F6,
            F7, F8, F9, F10,
            F11, F12, F13, F14,
            F15, PAUSE

        };

        Keyboard();
        ~Keyboard();

        bool IsKeyPressed(Keyboard::KEY key);

    private:
        void Init(void);

        sf::Keyboard* m_Keyboard;

	};

}

