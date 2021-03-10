#pragma once


#include <vector>
#include <memory>

#include "../core/datatypes/UDT.h"
#include "../core/datatypes/DT.h"
#include "../core/datatypes/Vector2.h"
#include "../physics/collider/svcollider/ColliderMesh.h"


#define EMPTY_CONTAINER			1
#define ELEMENT_FOUND			1
#define MAX_OBJECT_TYPES		4

#define DEF_MAP_DIMENSION_MIN	{ -5000.0f, -5000.0f }
#define DEF_MAP_DIMENSION_MAX	{ 5000.0f, 5000.0f }
#define DEF_MAP_DIMENSIONS		{ DEF_MAP_DIMENSION_MIN, DEF_MAP_DIMENSION_MAX }		


namespace Strave {

	namespace GUI { class UserInterface; }

	class Collider;
	class ObjectContainer;
	class GameObject2D;
	class WindowsWindow;
	class GameCamera2D;

	enum class OBJECT_TYPE;

	namespace nRenderer 
	{
		class RenderingConstraints;
		class DistanceRenderer;
		template<typename Type> class VectorContainer abstract;

		class Renderer 
		{
		private:
			typedef std::vector<GameObject2D*>				go2dl_t;	// game object 2d list
			typedef std::vector<GameObject2D*>::iterator	go2dl_iter;	// game object 2d iterator

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

			// draw functions
			void Draw(const Strave::Collider& col);
			void Draw(const ColliderMesh::Mesh& mesh, ColliderMesh::Type type);
			void DrawLine(Vector2f vertice[], const Uint16 vertexCount);

		private:
			void DrawBoxMesh(const ColliderMesh::Mesh& box);
			void DrawCircleMesh(const ColliderMesh::Mesh& circle);

		private:
			RenderingConstraints*				m_AssetsRenderingConstraints;
			std::shared_ptr<DistanceRenderer>	m_AssetsDistanceRenderer;
			std::shared_ptr<go2dl_t>*			m_BufferedAssets;

			GameCamera2D*						m_GameCamera2DReference;
			WindowsWindow*						m_WindowsWindow;

			std::vector<GameObject2D*>			m_GO2DContainer;
			std::vector<GUI::UserInterface*>	m_UIContainer;

		};

	}

}

