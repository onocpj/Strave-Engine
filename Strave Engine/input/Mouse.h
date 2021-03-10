#pragma once

#include "../core/datatypes/Vector2.h"
#include "../core/datatypes/DT.h"

#define MOUSE_DOUBLE_CLICK				2
#define MOUSE_DOUBLE_CLICK_DELAY		0.2		// seconds

#if defined(USE_SFML_INPUT)
	namespace sf { 
	
	class RenderWindow; 
	class Mouse;

}
#endif

namespace Strave {

	namespace GUI { class UserInterface; }
	class GameObject2D;
	class WindowsWindow;

	class SV_SANDBOX_API Mouse 
	{
    private:
		struct Cursor 
		{
			std::shared_ptr<sf::Mouse>	Reference;
			Vector2f					Position;
		};
		struct MouseDoubleClick 
		{
			float	LapsedTime;
			bool	Active;
		};
		struct MouseClick 
		{
			const GameObject2D*	ClickedObjectHolder;
			bool				ClickFlag;
		};

		typedef Cursor				cursor_t;
		typedef MouseDoubleClick	mdc_t;
		typedef MouseClick			mc_t;

	public:
        enum class Button 
		{
            Left = 0,       
            Right,      
            Middle
        };

		Mouse(WindowsWindow& window);
		~Mouse();

		static Mouse& Create(void);
		/// \brief Check if a mouse button is pressed
		///
		/// \param button - button to check
		///
		/// \return True if the button is pressed, false otherwise
		/// 
		bool IsButtonPressed(Mouse::Button button);
		/// \brief Check if a mouse button is pressed on object
		///
		/// \param button - button to check
		/// \param obj - relative to this object
		///
		/// \return True if the button is pressed on object, false otherwise
		/// 
		bool IsButtonPressed(Mouse::Button button, const GUI::UserInterface& obj);
		bool IsButtonPressed(Mouse::Button button, const GameObject2D& obj);
		/// \brief Check if a mouse button is clicked
		///
		/// \param button - button to check
		///
		/// \return True if the button is pressed, false otherwise
		/// 
		bool Click(Mouse::Button button);
		/// \brief Check if a mouse button clicked to object
		///
		/// \param button - button to check
		/// \param obj - relative to this object
		///
		/// \return True if the button is clicked on object, false otherwise
		/// 
		bool Click(Mouse::Button button, const GUI::UserInterface& obj);
		bool Click(Mouse::Button button, const GameObject2D& obj);
		/// \brief Check if a mouse button is doubble clicked
		///
		/// \param button - button to check
		///
		/// \return True if the button is doubble clicked, false otherwise
		/// 
		bool DoubleClick(Mouse::Button button);
		/// \brief Check if a mouse button is doubble clicked on object
		///
		/// \param button - button to check
		/// \param obj - relative to this object
		///
		/// \return True if the button is doubble clicked on object, false otherwise
		/// 
		bool DoubleClick(Mouse::Button button, const GUI::UserInterface& obj);
		bool DoubleClick(Mouse::Button button, const GameObject2D& obj);
		/// \brief Check if a mouse button is doubble clicked is performed, in specific delay
		///
		/// \param button - button to check
		/// \param clickDelay - delay to second click
		///
		/// \return True if the button is doubble clicked in specific delay, false otherwise
		/// 
		bool DoubleClick(Mouse::Button button, float clickDelay);
		/// \brief Check if a mouse button is doubble clicked is performed on specific object, in specific delay
		///
		/// \param button - button to check
		/// \param clickDelay - delay to second click
		/// \param obj - relative to this object
		///
		/// \return True if the button is doubble clicked on object in specific delay, false otherwise
		/// 
		bool DoubleClick(Mouse::Button button, float clickDelay, const GUI::UserInterface& obj);
		bool DoubleClick(Mouse::Button button, float clickDelay, const GameObject2D& obj);
		/// \brief Check if a mouse button was released
		///
		/// \param button - button to check
		///
		/// \return True if the button was released, false otherwise
		/// 
		bool Released(Mouse::Button button);
		/// \brief Check if a mouse button was released on object
		///
		/// \param button - button to check
		/// \param obj - relative to this object
		///
		/// \return True if the button was released on specific object, false otherwise
		/// 
		bool Released(Mouse::Button button, const GUI::UserInterface& obj);
		bool Released(Mouse::Button button, const GameObject2D& obj);
		/// \brief Check if a mouse button is holded
		///
		/// \param button - button to check
		///
		/// \return True if the button is holded, false otherwise
		/// 
		bool Hold(Mouse::Button button);
		/// \brief Check if a mouse button is holded on specific object
		///
		/// \param button - button to check
		/// \param obj - relative to this object
		///
		/// \return True if the button is holded on specific object, false otherwise
		/// 
		bool Hold(Mouse::Button button, const GUI::UserInterface& obj);
		bool Hold(Mouse::Button button, const GameObject2D& obj);
		/// \brief Check if a mouse curcor is hovering object
		///
		/// \param obj - relative to this object
		///
		/// \return True if the cursor is hovering object, false otherwise
		/// 
		bool Hover(const GUI::UserInterface& obj);
		bool Hover(const GameObject2D& obj);
		/// \brief Check if object is draged by cursor
		///
		/// \param obj - relative to this object
		///
		/// \return True if cursor is draging object, false otherwise
		/// 
		bool Drag(GUI::UserInterface& obj);
		bool Drag(GameObject2D& obj);

    private:
		void Init(WindowsWindow& window);

		sf::RenderWindow*	m_WindowReference;
		cursor_t			m_Cursor;
		mdc_t				m_DoubleClick;
		mc_t				m_Click;
	};

}

