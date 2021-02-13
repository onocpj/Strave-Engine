#pragma once

#include "Core.h"

#if defined(USE_SFML)
	#include <SFML/Window.hpp>
	#include <SFML/Graphics/RenderWindow.hpp>
	#include <SFML/Graphics.hpp>

	#define CLOCK		sf::Clock
	#define TIME		sf::Time
	#define VIDEO_MODE	sf::VideoMode
	#define RENDER_WINDOW	sf::RenderWindow
	#define EVENT		sf::Event
#endif

#if defined(USE_STRAVE)
	#include "Timer.h"
	#include "Window.h"


	class Event {};

	#define CLOCK		Strave::Timer::Clock
	#define TIME		Strave::Timer::Time
	#define VIDEO_MODE	Strave::WindowResolution;
	#define RENDER_WINDOW	Strave::Window
	#define EVENT		Event;
#endif

#if defined(SV_PLATFORM_WINDOWS)
	#include <string>
	#include <functional>
	#include <memory>
#endif

#include "Vector2.h"


// Definition of undefined attributes
#define UNDEF_WINDOW_TRIGGER	{ 0, 0 }
#define QUIT_APP_INVOKED	1


namespace Strave {

	class Application;
	class WindowsWindow;
	class Mouse;

	class EngineClocks abstract {
	public:
		static float deltaTime;

	private:
		friend WindowsWindow;

		static CLOCK	m_Clock;
		static TIME	m_Time;

		/**
			Updates engine clocks.
			This function is called each frame so it is defined as inline for performance efficiency
		*/
		inline static void Update(void) {

		#if defined(USE_SFML)
			EngineClocks::m_Time = EngineClocks::m_Clock.getElapsedTime();
			EngineClocks::deltaTime = (float)EngineClocks::m_Time.asSeconds();
			EngineClocks::m_Clock.restart();
		#endif
		#if defined(USE_STRAVE)
			EngineClocks::m_Time = EngineClocks::m_Clock.GetElapsedTime();
			EngineClocks::deltaTime = EngineClocks::m_Time.AsMicroseconds();
			EngineClocks::m_Clock.Reset();
		#endif

		} // critical function (runtime function)

	};
	class Resolution {
	public:
		Resolution(unsigned int width, unsigned int height);
		~Resolution();

		/**
			Returns resolution width
		*/
		inline unsigned int GetWidth() { 
			
			return m_Width; 
		
		}
		/**
			Returns resolution height
		*/
		inline unsigned int GetHeight() { 
			
			return m_Height; 
		
		}

	private:
		unsigned int m_Width;
		unsigned int m_Height;

	};
	class Framerate {

	public:
		Framerate(unsigned int framerate);
		~Framerate();

		/**
			Returns framerate cap if is set
		*/
		inline unsigned int GetFramerateCap() { return m_FramerateCap; }

	private:
		unsigned int m_FramerateCap;

	};
	class WindowsWindow {

	private:
		friend Mouse;

		using NoParamFunc_t	= std::function<void(void)>;
		using Invoker_t		= bool;

	public:
		WindowsWindow();
		WindowsWindow(const VIDEO_MODE& resolution, const Framerate& framerate, std::string title, const std::function<void (void)>& fStart, const std::function<void (void)>& fUpdate, const std::function<void(void)>& fEnd);
		~WindowsWindow();

#if defined(USE_SFML)
		/**
			Sets default view for render window
		*/
		inline void DefaultView(void) const { 
			
			m_RenderWindow->setView(m_RenderWindow->getDefaultView()); 
		
		} // critical function (runtime function)
		/**
			Draws object in window

			@param: obj - this object will be drawn in the window
		*/
		template<typename Obj_t> void Draw(const Obj_t& obj) const;															// critical function (runtime function)
		template<> inline void Draw<sf::Sprite>(const sf::Sprite& obj) const { m_RenderWindow->draw(obj); }			// critical function (runtime function)
		template<> inline void Draw<sf::RectangleShape>(const sf::RectangleShape& obj) const { m_RenderWindow->draw(obj); }	// critical function (runtime function)
		/**
			Sets camera viewport to any kind of view

			@param: view - window will be aiming for this viewport
		*/
		inline void SetView(const sf::View& view) const { 

			m_RenderWindow->setView(view); 

		}
#endif
		/**
			Returns resolution of the window
		*/
		inline sf::VideoMode GetResolution(void) const { 
			
			return m_Resolution; 
		
		}
		/**
			Returns resolution of the window in float
		*/
		inline Strave::Vector2f GetResolutionInFloats(void) const { 
			
			return Strave::Vector2f(
				(float)m_Resolution.width, 
				(float)m_Resolution.height
			); 
		
		}

		/**
			Executes created window
		*/
		void Execute(void);

	protected:
		inline sf::RenderWindow& GetWindow(void) {

			return *m_RenderWindow;

		}

	private:
		/**
			Invokes application closing state.
			When this event occurs, engine will end all tasks to be able to exit aplication correctly
		*/
		void InvokeCloseState(void);

		std::shared_ptr<RENDER_WINDOW>	m_RenderWindow;
		std::shared_ptr<EVENT>		m_Event;
		std::string			m_Title;
		VIDEO_MODE			m_Resolution;
		Framerate			m_Framerate;
		Invoker_t			m_ExitInvoker;

		NoParamFunc_t m_WindowStart;
		NoParamFunc_t m_WindowUpdate;
		NoParamFunc_t m_WindowEnd;

	};

}

