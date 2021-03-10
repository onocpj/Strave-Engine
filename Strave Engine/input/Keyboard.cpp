#include "Keyboard.h"

#include "../core/datatypes/UDT.h"

#if defined(USE_SFML_INPUT)
	#include <SFML\Window\Keyboard.hpp>
#endif

#if defined(SV_PLATFORM_WINDOWS)
	#include <Windows.h>
	#include <conio.h>
#endif

namespace Strave {

	Keyboard::Keyboard()
		: m_Keyboard(UNDEF_PTR)
	{
		Init();
	}
	Keyboard::~Keyboard() {}

	void Keyboard::Init(void) 
	{
		m_Keyboard = new sf::Keyboard();
	}
#if defined(USE_SFML_INPUT)
	bool Keyboard::IsKeyPressed(Keyboard::KEY key) {

		return m_Keyboard->isKeyPressed(static_cast<sf::Keyboard::Key>(key));

	}
#endif
#if defined(USE_STRAVE_INPUT)
	bool Keyboard::IsKeyPressed(Keyboard::KEY key) {

		if (_kbhit()) {

			int t_GetchedKey = UNDEFINED_KEY;
			Keyboard::KEY t_PressedKey = Keyboard::KEY::UNDEFINED;

			t_PressedKey = static_cast<Keyboard::KEY>(t_GetchedKey);

			if (t_PressedKey == key) {
				return true;
			}

		}

		return false;

	}
#endif

}
