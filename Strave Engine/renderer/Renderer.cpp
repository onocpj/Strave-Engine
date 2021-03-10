#include "Renderer.h"

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "../interface/UserInterface.h"
#include "../window/WindowsWindow.h"
#include "../entities/GameObject2D.h"
#include "../camera/GameCamera2D.h"
#include "../core/logger/Log.h"
#include "../core/datatypes/UDT.h"
#include "../renderer/DistanceRendering.h"
#include "../entities/Player2D.h"
#include "../renderer/RenderingConstraints.h"
#include "../entities/container/ObjectContainer.h"
#include "../core/datatypes/Convert.h"
#include "../physics/collider/svcollider/SVCollider.h"

#include <iostream>


namespace Strave {

	namespace nRenderer {

		//========================================================================================
		//									class: Renderer
		//========================================================================================

		Renderer::Renderer()
			: m_AssetsRenderingConstraints(UNDEF_PTR), m_AssetsDistanceRenderer(UNDEF_SMARTPTR), 
			m_BufferedAssets(UNDEF_SMARTPTR),m_GameCamera2DReference(UNDEF_PTR), m_WindowsWindow(UNDEF_PTR)
		{
		
		}
		Renderer::Renderer(const Strave::WindowsWindow& window, RenderingConstraints& constraints)
			: m_AssetsRenderingConstraints(&constraints), m_AssetsDistanceRenderer(UNDEF_SMARTPTR), 
			m_BufferedAssets(UNDEF_SMARTPTR), m_GameCamera2DReference(UNDEF_PTR), m_WindowsWindow(UNDEF_PTR)
		{
			Init(window);
		}

		void Renderer::Init(const Strave::WindowsWindow& window) 
		{
			m_WindowsWindow = const_cast<Strave::WindowsWindow*>(&window);
		}

		void Renderer::PostInit(void) 
		{
			OBJECT_TYPE t_InsertTypes[] = {
				OBJECT_TYPE::BASIC, 
				OBJECT_TYPE::RIGID_BODY, 
				OBJECT_TYPE::PLAYER,
				OBJECT_TYPE::UI
			};

			InsertObjectsToScene(*ObjectContainer::Get(), t_InsertTypes);

			m_AssetsDistanceRenderer = std::make_shared<DistanceRenderer>(
				*m_AssetsRenderingConstraints,
				m_GO2DContainer,				
				Vector4f(DEF_MAP_DIMENSIONS)	// TEMPORARY SOLUTION !!! USED ONLY FOR TEST
			);
		}

		void Renderer::RenderScene() 
		{
			// render all objects that are visible on screen
			if (m_BufferedAssets) 
			{
				for (auto object = (*m_BufferedAssets)->begin(), end = (*m_BufferedAssets)->end(); object != end; ++object)
				{
					if (Shapes::fPoint::IsInRange<float>(
						Shapes::fPoint::DistanceBetweenPoints<float>(m_AssetsDistanceRenderer->GetRenderSection()->GetPointPosition(), (*object)->GetSpritePosition()),
						m_AssetsDistanceRenderer->GetRenderSection()->GetRenderDistance()
						))
					{
						Animation::Update(**object);
						m_WindowsWindow->Draw(*((*object)->GetModel<sf::Sprite>()));
					}
				}
			}

			m_WindowsWindow->DefaultView();	// reset window view to defautl view

			// render user interface
			for (auto ui = m_UIContainer.begin(), end = m_UIContainer.end(); ui != end; ++ui) 
			{
				if ((*ui)->GetShowState() == true) 
					(*ui)->Draw();
			}

			// if camera reference is referenced to specific object
			if (m_GameCamera2DReference != UNDEF_PTR)
			{
				m_GameCamera2DReference->UpdateViewport(*m_WindowsWindow);													// update viewport for this object

				m_AssetsDistanceRenderer->GetRenderSection()->UpdatePointPosition(m_GameCamera2DReference->GetPosiiton());	// update rendering area position
				RENDERING_UPDATE(m_AssetsDistanceRenderer->GetRenderSection());												// update rendering section
				m_BufferedAssets = &m_AssetsDistanceRenderer->GetRenderSection()->GetObjectsToRender();
			}
		}

		void Renderer::InsertObjectsToScene(ObjectContainer& objContainer, OBJECT_TYPE objTypes[MAX_OBJECT_TYPES]) 
		{
			for (Uint64 index = 0; index != MAX_OBJECT_TYPES; index++) {
				if (objTypes[index] != OBJECT_TYPE::UNDEFINED) {

					switch (objTypes[index]) {
					case OBJECT_TYPE::BASIC:
						
						m_GO2DContainer.insert(m_GO2DContainer.end(),
							objContainer.GetContainer<GameObject2D>().GetElements().begin(),
							objContainer.GetContainer<GameObject2D>().GetElements().end()
						);

						break;
					case OBJECT_TYPE::PLAYER:

						m_GO2DContainer.insert(m_GO2DContainer.end(),
							objContainer.GetContainer<Player2D>().GetElements().begin(),
							objContainer.GetContainer<Player2D>().GetElements().end()
						);

						break;
					case OBJECT_TYPE::RIGID_BODY:

						m_GO2DContainer.insert(m_GO2DContainer.end(),
							objContainer.GetContainer<RigidBody2D>().GetElements().begin(),
							objContainer.GetContainer<RigidBody2D>().GetElements().end()
						);

						break;
					case OBJECT_TYPE::UI:

						m_UIContainer.insert(m_UIContainer.end(),
							objContainer.GetContainer<GUI::UserInterface>().GetElements().begin(),
							objContainer.GetContainer<GUI::UserInterface>().GetElements().end()
						);

						break;

					default:
						SV_CORE_WARN("function Renderer::InsertObjectToScene: Trying to insert udenfined object type");
						break;
					}

				}
			}
		}

		void Renderer::EraseObjectFromScene(OBJECT_TYPE objType, const GameObject2D& obj) 
		{
			switch (objType) {
			case OBJECT_TYPE::BASIC:
			case OBJECT_TYPE::RIGID_BODY:
			case OBJECT_TYPE::PLAYER:

				// If container is not empty
				if (!m_GO2DContainer.empty()) {

					// Cycle whole conatiner
					for (std::vector<GameObject2D*>::iterator iter = m_GO2DContainer.begin(), iterEnd = m_GO2DContainer.end(); iter != iterEnd; iter++) {
						// If element is equal to object passed as parameter
						if ((*iter) == &obj) {
							// Erase this element from container
							m_GO2DContainer.erase(iter);

							SV_CORE_TRACE("ObjectContainer::VectorContainer<Type>::EraseFromContainer(const Type&): Object erased from conatiner");

							break;

						} else { // // If element is not equal to object passed as parameter

						 // Inform about it
							SV_CORE_TRACE("ObjectContainer::VectorContainer<Type>::EraseFromContainer(const Type&): Object not found in container");
						}
					}

				}
				else { // If container is empty

				 // Inform about it
					SV_CORE_INFO("ObjectContainer::VectorContainer<Type>::EraseFromContainer(const Type&): Container is empty");
				}

				break;

			case OBJECT_TYPE::UI:

				// If container is not empty
				if (!m_UIContainer.empty()) {

					// Cycle whole conatiner
					for (std::vector<GUI::UserInterface*>::iterator iter = m_UIContainer.begin(), iterEnd = m_UIContainer.end(); iter != iterEnd; iter++) {
						// If element is equal to object passed as parameter
						if ((*iter) == &obj) {
							// Erase this element from container
							m_UIContainer.erase(iter);

							SV_CORE_TRACE("ObjectContainer::VectorContainer<Type>::EraseFromContainer(const Type&): Object erased from conatiner");

							break;

						}
						else { // // If element is not equal to object passed as parameter

							// Inform about it
							SV_CORE_TRACE("ObjectContainer::VectorContainer<Type>::EraseFromContainer(const Type&): Object not found in container");
						}
					}

				}
				else { // If container is empty

				 // Inform about it
					SV_CORE_INFO("ObjectContainer::VectorContainer<Type>::EraseFromContainer(const Type&): Container is empty");
				}

				break;

			default:
				SV_CORE_WARN("function Renderer::EraseObjectFromScene: Trying to erase udenfined object type");
				break;
			}
		}

		void Renderer::SetGameCamera2DReference(const Strave::GameCamera2D& gameCamera) 
		{
			m_GameCamera2DReference = const_cast<Strave::GameCamera2D*>(&gameCamera);
		}

		void Renderer::Delete(void) const 
		{
			if (this != UNDEF_PTR) {
				delete this;

				SV_CORE_INFO("Renderer::Delete(): Renderer removed from memory");

			} else {
				SV_CORE_WARN("Renderer::Delete(): Unable to remove renderer from memory");
			}
		}

		void Renderer::Draw(const Strave::Collider& col)
		{
			Uint16 meshCount = col.GetMeshCount();
			ColliderMesh::Mesh* mesh = col.GetMesh();

			for (Uint16 i = 0; i != col.GetMeshCount(); i++)
				Renderer::Draw(mesh[i], mesh[i].GetMeshType());		
		}

		void Renderer::Draw(const ColliderMesh::Mesh& mesh, ColliderMesh::Type type)
		{
			switch (type)
			{
			case ColliderMesh::Type::Box: Renderer::DrawBoxMesh(mesh); break;
			case ColliderMesh::Type::Circle: Renderer::DrawCircleMesh(mesh); break;
			case ColliderMesh::Type::Undefined: break;
			}
		}

		void Renderer::DrawLine(Vector2f vertice[], const Uint16 vertexCount)
		{
			Uint16 nov = UNDEF_INT16;
			if ((nov = (vertexCount % 2)) == 0)
			{
				sf::Vertex* vertices = new sf::Vertex[nov]();

				for (Uint16 i = 0; i < vertexCount; i++)
					vertices[i] = sf::Vertex(Convert::ToVector2(vertice[i]), sf::Color::White, Convert::ToVector2(vertice[i + 1]));

				m_WindowsWindow->Draw(vertices, vertexCount, sf::Lines);
			}
		}

		void Renderer::DrawBoxMesh(const ColliderMesh::Mesh& box)
		{
			Vector2f size = box.GetMeshSize();
			Vector2f position = box.GetPosition();
			Vector2f initPosition = {
				position.x - (size.x / 2),
				position.y - (size.y / 2)
			};
			Vector2f vertices[] = {
				Vector2f(initPosition.x, initPosition.y + size.y),			// A
				Vector2f(initPosition.x, initPosition.y),					// B
				Vector2f(initPosition.x + size.x, initPosition.y),			// C
				Vector2f(initPosition.x + size.x, initPosition.y + size.y), // D
				Vector2f(initPosition.x, initPosition.y)					// B
			};

			Renderer::DrawLine(vertices, 5);

			// Draws path (A -> B -> C -> D -> B)
			/*
			*	B				 C
			*	__________________
			*	|*				 |
			*	|	*			 |
			*	|		*		 |
			*	|			*	 |
			*	|_______________*|
			* 
			*	A				 D
			* 
			*/
		}

		void Renderer::DrawCircleMesh(const ColliderMesh::Mesh& circle)
		{
			float radius = circle.GetMeshSize().x;
			sf::CircleShape circleShape(radius);

			m_WindowsWindow->Draw(circleShape);
		}

	}

}
