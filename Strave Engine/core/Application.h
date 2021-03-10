#pragma once

#define UNDEF_APP_TRIGGER		{ 0, 0 }


namespace Strave {

	namespace nRenderer { class Renderer; }

	class WindowsWindow;
	class Application 
	{
	public:
		Application();
		virtual ~Application();

		void Init(Strave::WindowsWindow* window);

		virtual void Start(void) const	= 0;
		virtual void Update(void) const	= 0;
		virtual void End(void) const	= 0;

		inline const bool& GetOnStartState(void) { return m_ApplicationTiggerState.OnStart; }
		inline const bool& GetOnExitState(void) { return m_ApplicationTiggerState.OnExit; }

		inline Strave::WindowsWindow* GetWindow(void) const { return m_MainWindow; }
		inline Strave::nRenderer::Renderer* GetRenderer(void) const { return m_Renderer; }

	protected:
		inline void SetOnStartState(bool state) { m_ApplicationTiggerState.OnStart = state; }
		inline void SetOnExitState(bool state) { m_ApplicationTiggerState.OnExit = state; }

	private:
		friend WindowsWindow;
		typedef struct ApplicationTriggerState
		{
			bool OnStart;
			bool OnExit;
		} apt_t;

	private:
		apt_t							m_ApplicationTiggerState;
		Strave::WindowsWindow*			m_MainWindow;
		Strave::nRenderer::Renderer*	m_Renderer;

	};

	Strave::Application* CreateApplication();

}

