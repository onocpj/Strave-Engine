#pragma once

#include "GameObject2D.h"

#include <vector>


#define RB_CONTAINER_2D_EMPTY_PTR	nullptr
#define RB_CONTAINER_2D_EMPTY		1
#define RB_CONTAINER_2D_NOT_EMPTY	0

#define RB_ENABLED		true
#define RB_DISABLED		false
#define RB_ENABLE		true
#define RB_DISABLE		false

#define RB_IN_COLLISION		true
#define RB_NOT_IN_COLLISION	false


namespace Strave {

	class Collider;
	class RigidBody2D;
	class RigidBody2DContainer abstract
	{
	private:
		friend RigidBody2D;

		static std::vector<RigidBody2D*> m_ObjectContainer;

		static bool IsEmpty(void);
		static void PushToContainer(const RigidBody2D& obj);
		static void EraseFromContainer(const RigidBody2D& obj);
	};
	class SV_SANDBOX_API RigidBody2D : public GameObject2D
	{
	public:
		inline virtual ~RigidBody2D() {}

		static RigidBody2D* Create(sf::Texture& texture, const Strave::Vector2f& position, const Strave::Vector2f& spriteScale = GAME_OBJECT_2D_SCALE);
		static void Enable(RigidBody2D& obj);
		static void Disable(RigidBody2D& obj);
		static void CollisionDetection(void);
		static void OnTriggerCollision(RigidBody2D& obj);
		inline bool InCollision(void) const { 
			
			return m_InCollision; 
		
		}

		// collider remake functions
		inline void AssignCollider(Collider& collider)
		{
			m_ColliderReference = &collider;
		}
		void EnableCollider(void);
		void DisableCollider(void);
		void ShowCollider(void);
		void HideCollider(void);
		// collider remake functions

	protected:
		RigidBody2D(OBJECT_TYPE objectType, std::weak_ptr<sf::Texture> texture, std::weak_ptr<sf::Sprite> sprite, std::weak_ptr<sf::RectangleShape> rectSprite, const Strave::Vector2f& position, const Strave::Vector2f& spriteScale);
		RigidBody2D();

	private:
		inline bool RigidBodyEnabled(void) const 
		{ 
			return m_RigidBodyEnabled; 
		}
		inline void EnableRigidBody(bool state) 
		{ 
			m_RigidBodyEnabled = state;
		}
		inline void SetCollisionStatus(bool state) 
		{ 
			m_InCollision = state; 
		}

	private:
		bool		m_RigidBodyEnabled;
		bool		m_InCollision;
		Collider*	m_ColliderReference;
	};

}

