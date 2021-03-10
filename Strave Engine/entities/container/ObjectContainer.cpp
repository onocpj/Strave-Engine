#include "ObjectContainer.h"

#include "../GameObject2D.h"
#include "../RigidBody2D.h"
#include "../Player2D.h"
#include "../../interface/UserInterface.h"
#include "../../core/logger/Log.h"

namespace Strave {

	ObjectContainer* ObjectContainer::s_Container = new ObjectContainer();

	ObjectContainer::ObjectContainer() 
	{}
	ObjectContainer::~ObjectContainer() 
	{}

	void ObjectContainer::Insert(OBJECT_TYPE objType, GameObject2D& object) {

		switch (objType) {

		case OBJECT_TYPE::BASIC:
			m_GameObjectContainer.PushToContainer(object);
			break;

		case OBJECT_TYPE::RIGID_BODY:
			m_RigidBodyContainer.PushToContainer(*static_cast<RigidBody2D*>(&object));
			break;

		case OBJECT_TYPE::PLAYER:
			m_PlayerContainer.PushToContainer(*static_cast<Player2D*>(&object));
			break;

		case OBJECT_TYPE::UI_TABLE:
		case OBJECT_TYPE::UI_TEXTBOX:
		case OBJECT_TYPE::UI_LABEL:
		case OBJECT_TYPE::UI_BUTTON:
			m_UIContainer.PushToContainer(*static_cast<GUI::UserInterface*>(&object));
			break;

		}

	}
	void ObjectContainer::Erase(OBJECT_TYPE objType, GameObject2D& object) {

		switch (objType) 
		{
		case OBJECT_TYPE::BASIC:
			m_GameObjectContainer.EraseFromContainer(object);
			break;

		case OBJECT_TYPE::RIGID_BODY:
			m_RigidBodyContainer.EraseFromContainer(*static_cast<RigidBody2D*>(&object));
			break;

		case OBJECT_TYPE::PLAYER:
			m_PlayerContainer.EraseFromContainer(*static_cast<Player2D*>(&object));
			break;

		case OBJECT_TYPE::UI_TABLE:
		case OBJECT_TYPE::UI_TEXTBOX:
		case OBJECT_TYPE::UI_LABEL:
		case OBJECT_TYPE::UI_BUTTON:
			m_UIContainer.EraseFromContainer(*static_cast<GUI::UserInterface*>(&object));
			break;
		}

	}
	unsigned int ObjectContainer::Size(OBJECT_TYPE objType) const {

		switch (objType) 
		{
		case OBJECT_TYPE::BASIC:
			return m_GameObjectContainer.GetSize();

		case OBJECT_TYPE::RIGID_BODY:
			return m_RigidBodyContainer.GetSize();

		case OBJECT_TYPE::PLAYER:
			return m_PlayerContainer.GetSize();

		case OBJECT_TYPE::UI_TABLE:
		case OBJECT_TYPE::UI_TEXTBOX:
		case OBJECT_TYPE::UI_LABEL:
		case OBJECT_TYPE::UI_BUTTON:
			return m_UIContainer.GetSize();

		default:
			return 0;
		}

	}

	//========================================================================================
	//								class: VectorContainer
	//========================================================================================

	template<typename Type> ObjectContainer::VectorContainer<Type>::VectorContainer()
	{}
	template<typename Type> ObjectContainer::VectorContainer<Type>::~VectorContainer() 
	{}

	template<typename Type> bool ObjectContainer::VectorContainer<Type>::IsEmpty(void) const {

		if (m_Container.size() == 0) {
			return EMPTY_CONTAINER;
		}
		else {
			return !EMPTY_CONTAINER;
		}

	}
	template<typename Type> void ObjectContainer::VectorContainer<Type>::PushToContainer(const Type& val) {

		bool t_ElementFound;

		// If RigidBody2DContainer is empty
		if (this->IsEmpty()) {
			this->m_Container.push_back(const_cast<Type*>(&val)); // Push straight object into container

		}
		else { // If RigidBody2DContainer is not empty

		// Cycle whole conatiner
			for (Iterrator_t iter = this->m_Container.begin(), iterEnd = this->m_Container.end(); iter != iterEnd; iter++) {
				// If element is not equal to object passed as parameter
				if ((*iter) != &val) {
					// Element not found
					t_ElementFound = !ELEMENT_FOUND;

				}
				else { // if element is equal to object passed as parameter
				 // Element found
					t_ElementFound = ELEMENT_FOUND;
					break;
				}
			}

			// if element was not found in container
			if (t_ElementFound == !ELEMENT_FOUND) {
				// Push object into container
				this->m_Container.push_back(const_cast<Type*>(&val));
			}
			else { // If element was found in container

			 // Inform about it
				SV_CORE_WARN("ObjectContainer::VectorContainer<Type>::PushToContainer(const Type&): Object is already in the container");
			}

		}

	}
	template<typename Type> void ObjectContainer::VectorContainer<Type>::EraseFromContainer(const Type& val) {

		// If container is not empty
		if (!this->IsEmpty()) {

			// Cycle whole conatiner
			for (Iterrator_t iter = this->m_Container.begin(), iterEnd = this->m_Container.end(); iter != iterEnd; iter++) {
				// If element is equal to object passed as parameter
				if ((*iter) == &val) {
					// Erase this element from container
					this->m_Container.erase(iter);

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

	}

}

