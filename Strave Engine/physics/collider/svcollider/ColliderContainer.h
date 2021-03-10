#pragma once

#include "../../../core/datatypes/BasicContainer.h"
#include "../../../core/datatypes/BasicContainer.cpp"

namespace Strave
{
	class RigidBody2D;
	class Collider;
	class ColliderContainer final : public BasicContainer<Collider>
	{
	private:
		friend Collider;
		static ColliderContainer* s_Collider;

	public:
		ColliderContainer() = default;
		~ColliderContainer() = default;
		ColliderContainer(const ColliderContainer&) = delete;

	protected:
		inline static ColliderContainer& Get(void)
		{ 
			return *s_Collider; 
		}
		inline void Delete(void) const 
		{ 
			delete s_Collider; 
		}
	};
}

