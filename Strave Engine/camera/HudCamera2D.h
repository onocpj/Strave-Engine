#pragma once

#include <vector>


namespace Strave {

	class GameObject2D;
	class WindowsWindow;

	class HudCamera2D {

	public:
		HudCamera2D();
		~HudCamera2D();

		void Update(const Strave::WindowsWindow &window) const;
		void InsertElement(const Strave::GameObject2D& obj);
		void EraseElement(const Strave::GameObject2D& obj);

	private:
		std::vector<Strave::GameObject2D*> m_ObjectContainer;

	};

}

