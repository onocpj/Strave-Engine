#pragma once


#include <vector>
#include <memory>

#include "UDT.h"


#define EMPTY_CONTAINER		1
#define ELEMENT_FOUND		1
#define MAX_OBJECT_TYPES	4

#define DEF_MAP_DIMENSION_MIN	{ -5000.0f, -5000.0f }
#define DEF_MAP_DIMENSION_MAX	{ 5000.0f, 5000.0f }
#define DEF_MAP_DIMENSIONS	{ DEF_MAP_DIMENSION_MIN, DEF_MAP_DIMENSION_MAX }		


namespace Strave {

	namespace GUI { class UserInterface; }

	class ObjectContainer;
	class GameObject2D;
	class WindowsWindow;
	class GameCamera2D;

	enum class OBJECT_TYPE;

	namespace NRenderer {

		class RenderingConstraints;
		class DistanceRenderer;
		template<typename Type> class VectorContainer abstract;

		class Renderer {

		private:
			typedef std::vector<GameObject2D*>		GO2DList_t;
			typedef std::vector<GameObject2D*>::iterator	GO2DList_iter;

		public:
			Renderer();
			Renderer(const WindowsWindow& window, RenderingConstraints& constraints);
			inline ~Renderer() {}

			void Init(const WindowsWindow& window);
			void PostInit(void);

			void RenderScene(); // critical function (runtime function)

			void InsertObjectsToScene(ObjectContainer& objContainer, OBJECT_TYPE objTypes[MAX_OBJECT_TYPES]);
			void EraseObjectFromScene(OBJECT_TYPE objType, const GameObject2D& obj);
			void SetGameCamera2DReference(const GameCamera2D& gameCamera);
			void Delete(void) const;

		private:
			RenderingConstraints*			m_AssetsRenderingConstraints;
			std::shared_ptr<DistanceRenderer>	m_AssetsDistanceRenderer;
			std::shared_ptr<GO2DList_t>*		m_BufferedAssets;

			GameCamera2D*				m_GameCamera2DReference;
			WindowsWindow*				m_WindowsWindow;

			std::vector<GameObject2D*>		m_GO2DContainer;
			std::vector<GUI::UserInterface*>	m_UIContainer;

		};

	}

}

