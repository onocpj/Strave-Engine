#pragma once

#include <vector>


#define EMPTY_CONTAINER			1
#define ELEMENT_FOUND			1


namespace Strave {

	enum class OBJECT_TYPE;

	class GameObject2D;
	class Player2D;
	class RigidBody2D;
	namespace GUI { class UserInterface; }

	class ObjectContainer 
	{
	private:
		static ObjectContainer* s_Container;
		template<typename Type> class VectorContainer 
		{
		public:
			VectorContainer();
			virtual ~VectorContainer();

			bool IsEmpty(void) const;
			void PushToContainer(const Type& val);
			void EraseFromContainer(const Type& val);
			inline unsigned int GetSize(void) const 
			{ 
				return m_Container.size(); 
			}
			inline std::vector<Type*>& GetElements(void) 
			{ 
				return m_Container; 
			}

		private:
			typedef typename std::vector<Type*>::iterator Iterrator_t;

		private:
			std::vector<Type*> m_Container;
		};

	public:
		ObjectContainer();
		~ObjectContainer();

		inline static ObjectContainer* Get() 
		{ 
			return s_Container; 
		}
		void Insert(OBJECT_TYPE objType, GameObject2D& object);
		void Erase(OBJECT_TYPE objType, GameObject2D& object);
		unsigned int Size(OBJECT_TYPE objType) const;
		template<typename Type> VectorContainer<Type>& GetContainer(void);
		template<> inline VectorContainer<Player2D>& GetContainer(void) { 
			return m_PlayerContainer; 
		}
		template<> inline VectorContainer<GameObject2D>& GetContainer(void) { 
			return m_GameObjectContainer; 
		}
		template<> inline VectorContainer<RigidBody2D>& GetContainer(void) { 
			return m_RigidBodyContainer; 
		}
		template<> inline VectorContainer<GUI::UserInterface>& GetContainer(void) { 
			return m_UIContainer; 
		}

	private:
		VectorContainer<Player2D>				m_PlayerContainer;
		VectorContainer<GameObject2D>			m_GameObjectContainer;
		VectorContainer<RigidBody2D>			m_RigidBodyContainer;
		VectorContainer<GUI::UserInterface>		m_UIContainer;
	};

}

