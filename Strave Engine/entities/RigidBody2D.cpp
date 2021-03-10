#include "RigidBody2D.h"

#include "../physics/collider/sfmlcollider/Collider.h"
#include "../physics/collider/svcollider/SVCollider.h"
#include "../core/logger/Log.h"
#include "../core/datatypes/Convert.h"

namespace Strave {

	std::vector<RigidBody2D*> RigidBody2DContainer::m_ObjectContainer;

	//========================================================================================
	//									class: RigidBody2DContainer
	//========================================================================================

	bool RigidBody2DContainer::IsEmpty(void) {

		if (m_ObjectContainer.size() == 0) {
			return RB_CONTAINER_2D_EMPTY;

		} else {
			return RB_CONTAINER_2D_NOT_EMPTY;
		}

	}
	void RigidBody2DContainer::PushToContainer(const RigidBody2D& obj) {

		bool t_ElementFound;

		// If RigidBody2DContainer is empty
		if (RigidBody2DContainer::IsEmpty()) {
			RigidBody2DContainer::m_ObjectContainer.push_back(const_cast<Strave::RigidBody2D*>(&obj)); // Push straight object into container

			SV_CORE_TRACE("Push: Object pushed to RigidBody2D container");

		} else { // If RigidBody2DContainer is not empty
			
			// Cycle whole conatiner
			for (std::vector<RigidBody2D*>::iterator iter = RigidBody2DContainer::m_ObjectContainer.begin(), iterEnd = RigidBody2DContainer::m_ObjectContainer.end(); iter != iterEnd; iter++) {
				// If element is not equal to object passed as parameter
				if ((*iter) != &obj) {
					// Element not found
					t_ElementFound = ELEMENT_NOT_FOUND_IN_CONTAINER;

				} else { // if element is equal to object passed as parameter
					// Element found
					t_ElementFound = ELEMENT_FOUND_IN_CONTAINER;
					break;
				}
			}

			// if element was not found in container
			if (t_ElementFound == ELEMENT_NOT_FOUND_IN_CONTAINER) {
				// Push object into container
				RigidBody2DContainer::m_ObjectContainer.push_back(const_cast<Strave::RigidBody2D*>(&obj));

				SV_CORE_TRACE("Push: Object pushed to RigidBody2D container");

			} else { // If element was found in container

				// Inform about it
				SV_CORE_INFO("Push: Object is already in the RigidBody2D container");
			}

		}

	}
	void RigidBody2DContainer::EraseFromContainer(const RigidBody2D& obj) {

		// If container is not empty
		if (!RigidBody2DContainer::IsEmpty()) {

			// Cycle whole conatiner
			for (std::vector<RigidBody2D*>::iterator iter = RigidBody2DContainer::m_ObjectContainer.begin(), iterEnd = RigidBody2DContainer::m_ObjectContainer.end(); iter != iterEnd; iter++) {
				// If element is equal to object passed as parameter
				if ((*iter) == &obj) {
					// Erase this element from container
					RigidBody2DContainer::m_ObjectContainer.erase(iter);

					SV_CORE_TRACE("Erase: Object erased from RigidBody2D conatiner");

					break;

				}
				else { // // If element is not equal to object passed as parameter

				 // Inform about it
					SV_CORE_TRACE("Erase: Object not found in RigidBody2D container");
				}
			}

		} else { // If container is empty

			// Inform about it
			SV_CORE_INFO("Erase: RigidBody2D container is empty");
		}

	}

	//========================================================================================
	//									class: RigidBody2D
	//========================================================================================

	RigidBody2D::RigidBody2D(OBJECT_TYPE objectType, std::weak_ptr<sf::Texture> texture, std::weak_ptr<sf::Sprite> sprite, std::weak_ptr<sf::RectangleShape> rectSprite, const Strave::Vector2f& position, const Strave::Vector2f& spriteScale)
		: GameObject2D(objectType, texture, sprite, rectSprite, position, spriteScale),
		m_ColliderReference(UNDEF_PTR), m_RigidBodyEnabled(RB_ENABLED), 
		m_InCollision(RB_NOT_IN_COLLISION)
	{
		RigidBody2DContainer::PushToContainer(*this);
	}
	RigidBody2D::RigidBody2D()
		: GameObject2D(),
		m_ColliderReference(UNDEF_PTR), m_RigidBodyEnabled(RB_ENABLED), 
		m_InCollision(RB_NOT_IN_COLLISION)
	{
		RigidBody2DContainer::PushToContainer(*this);
	}

	RigidBody2D* RigidBody2D::Create(sf::Texture& texture, const Strave::Vector2f& position, const Strave::Vector2f& spriteScale) {

		std::shared_ptr<sf::Sprite> t_Sprite2D = std::make_shared<sf::Sprite>();
		std::shared_ptr<sf::RectangleShape> t_RectSprite2D = std::make_shared<sf::RectangleShape>();

		t_Sprite2D->setTexture(texture);
		t_Sprite2D->setScale(Convert::ToVector2(spriteScale));
		t_Sprite2D->setPosition(Convert::ToVector2(position));

		t_RectSprite2D->setTexture(&texture);
		t_RectSprite2D->setSize(Convert::ToVector2(spriteScale));
		t_RectSprite2D->setPosition(Convert::ToVector2(position));

		return new RigidBody2D(OBJECT_TYPE::RIGID_BODY, std::make_shared<sf::Texture>(texture), t_Sprite2D, t_RectSprite2D, position, spriteScale);

	}

	void RigidBody2D::Enable(RigidBody2D& obj) {

		// If rigidbody is disabled for this object
		if (!obj.RigidBodyEnabled()) {
			RigidBody2DContainer::PushToContainer(obj);		// Push object to the container
			obj.EnableRigidBody(RB_ENABLE);			// Set rigibody state to enabled

			SV_CORE_TRACE("Enable: Rigidbody applied for this object");

		} else { // If rigidbody is already enabled for this object

			// Inform about it
			SV_CORE_INFO("Enable: Rigidbody is already applied for this object");
		}

	}

	void RigidBody2D::Disable(RigidBody2D& obj) {

		// If rigidbody is enabled for this object
		if (obj.RigidBodyEnabled()) {
			RigidBody2DContainer::EraseFromContainer(obj);	// Erase object to the container
			obj.EnableRigidBody(RB_DISABLE);			// Set rigibody state to disabled

			SV_CORE_TRACE("Disable: Rigidbody disabled for this object");

		} else { // If rigidbody is already disabled for this object

			// Inform about it
			SV_CORE_INFO("Disable: Rigidbody is already disabled for this object");
		}

	}

	void RigidBody2D::CollisionDetection(void) {

		// Cycle the rigibody container
		for (std::vector<RigidBody2D*>::iterator iter = RigidBody2DContainer::m_ObjectContainer.begin(), iterEnd = RigidBody2DContainer::m_ObjectContainer.end(); iter != iterEnd; iter++) {
			// Cycle the rigidbody container
			for (std::vector<RigidBody2D*>::iterator iter2 = RigidBody2DContainer::m_ObjectContainer.begin(); iter2 != RigidBody2DContainer::m_ObjectContainer.end(); iter2++) {
				// If iter is not same as inter2 
				if ((*iter) != (*iter2)) {
					// If pixel perfect test between iter and iter2 from rigidbody container is equal to true
					if (nCollider::BoundingBoxTest(*((*iter)->GetModel<sf::Sprite>()), *((*iter2)->GetModel<sf::Sprite>())) == RB_IN_COLLISION) {
						(*iter)->SetCollisionStatus(RB_IN_COLLISION);	// Set iter collision status to true
						(*iter2)->SetCollisionStatus(RB_IN_COLLISION);	// Set iter2 collision status to true

						break;

					} else { // If iter is same as inter2 

						// If iter is in collision
						if ((*iter)->InCollision()) {
							// Set iter collision status to false
							(*iter)->SetCollisionStatus(RB_NOT_IN_COLLISION);
						}
						// If iter2 is in collision
						if ((*iter2)->InCollision()) {
							// Set iter2 collision status to false
							(*iter2)->SetCollisionStatus(RB_NOT_IN_COLLISION);
						}
					}
				}
			}
		}

	}

	void RigidBody2D::OnTriggerCollision(RigidBody2D& obj) {

		// If rigid body for object passed as parameter is enabled
		if (obj.RigidBodyEnabled()) {
			// Cycle the rigidbody container
			for (std::vector<RigidBody2D*>::iterator iter = RigidBody2DContainer::m_ObjectContainer.begin(), iterEnd = RigidBody2DContainer::m_ObjectContainer.end(); iter != iterEnd; iter++) {
				// If object passed as parameter is not same as the current interator from container
				if ((*iter) != &obj) {
					// If objects are colliding
					if (nCollider::BoundingBoxTest(*(obj.GetModel<sf::Sprite>()), *((*iter)->GetModel<sf::Sprite>())) == RB_IN_COLLISION) {
						// Set object collision status to true
						obj.SetCollisionStatus(RB_IN_COLLISION);

						break;

					} else { // If objects are not colliding

						// if collision status of this object is true
						if (obj.InCollision()) {
							// Set object collision status to false
							obj.SetCollisionStatus(RB_NOT_IN_COLLISION);
						}

					}
				} 
			}
		}
	}

	void RigidBody2D::EnableCollider(void)
	{
		m_ColliderReference->m_ColliderEnabled = true;
	}

	void RigidBody2D::DisableCollider(void)
	{
		m_ColliderReference->m_ColliderEnabled = false;
	}

	void RigidBody2D::ShowCollider(void) 
	{ 
		m_ColliderReference->m_ShowCollider = true; 
	}

	void RigidBody2D::HideCollider(void) 
	{ 
		m_ColliderReference->m_ShowCollider = false; 
	}

}

