#pragma once

// Header mess
#include "GameObject2D.h"

#include <vector>


// Definition of empty constants
#define RIGIDBODY_CONTAINER_2D_EMPTY_PTR	nullptr
#define RIGIDBODY_CONTAINER_2D_EMPTY		1
#define RIGIDBODY_CONTAINER_2D_NOT_EMPTY	0

#define RIGIDBODY_ENABLED	true
#define RIGIDBODY_DISABLED	false
#define RIGIDBODY_ENABLE	true
#define RIGIDBODY_DISABLE	false

#define IN_COLLISION		true
#define NOT_IN_COLLISION	false


namespace Strave {

	// Predeclaration of classes
	class RigidBody2D;

	// Container for RigidBody2D objects
	class RigidBody2DContainer abstract {

	private:
		friend RigidBody2D;

		static std::vector<RigidBody2D*> m_ObjectContainer;

		static bool IsEmpty(void);
		static void PushToContainer(const RigidBody2D& obj);
		static void EraseFromContainer(const RigidBody2D& obj);

	};

	// RigidBody2D ables to create physical object that can physically interact with others objects
	class RigidBody2D : public GameObject2D {
	public:
		inline virtual ~RigidBody2D() {}

		/**
			Creates new rigid body object

			@param: fPath - file path of texture for the object
			@param: position - object starting position
			@param: spriteScale - scale of the texture
		*/
		static RigidBody2D* Create(sf::Texture& texture, const Strave::Vector2f& position, const Strave::Vector2f& spriteScale = GAME_OBJECT_2D_SCALE);
		/**
			Enables physicall interaction with other RigidBody2D objects

			@param: obj - applied for this ibject
		*/
		static void Enable(RigidBody2D& obj);
		/**
			Disables physicall interaction with other RigidBody2D objects

			@param: obj - applied for this ibject
		*/
		static void Disable(RigidBody2D& obj);
		/**
			Applies collision between each object
			Function is CPU heavy (n^2)
		*/
		static void CollisionDetection(void);
		/**
			Applies collision just for object passed as parameter with each object

			@param: obj - applied for this object
		*/
		static void OnTriggerCollision(RigidBody2D& obj);

		/**
			Returns true if object is in collision
		*/
		inline bool InCollision(void) const { 
			
			return m_InCollision; 
		
		}


	protected:
		RigidBody2D(OBJECT_TYPE objectType, std::weak_ptr<sf::Texture> texture, std::weak_ptr<sf::Sprite> sprite, std::weak_ptr<sf::RectangleShape> rectSprite, const Strave::Vector2f& position, const Strave::Vector2f& spriteScale);
		RigidBody2D();

	private:
		inline bool RigidBodyEnabled(void) const { return m_RigidBodyEnabled; }
		inline void EnableRigidBody(bool state) { m_RigidBodyEnabled = state; }
		inline void SetCollisionStatus(bool state) { m_InCollision = state; }

		bool m_RigidBodyEnabled;
		bool m_InCollision;

	};

}

