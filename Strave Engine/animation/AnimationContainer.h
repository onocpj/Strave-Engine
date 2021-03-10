#pragma once

#include <vector>
#include <string>
#include <map>

#include "../core/datatypes/DT.h"
#include "../core/datatypes/UDT.h"


#define UNDEFINED_INDEX -1


namespace Strave {

	class GameObject2D;
	class Animation;
	class AnimationContainer;

	class AnimationComplexType {
	private:
		friend AnimationContainer;

		Animation*			m_AnimationReference;
		const GameObject2D* m_ObjectReference;
		Uint32*				m_Animations;
		int					m_ActiveAnimation;

	protected:
		AnimationComplexType();

	public:
		~AnimationComplexType();

		inline Int32 GetActiveAnimation(void) const { 
			return this->m_ActiveAnimation; 
		}
		void SetActiveAnimationTo(Uint32 animation);
		void AssignDefaultAnimation();
		bool HaveActiveAnimation() const;
	};
	class AnimationContainer {
	private:
		static AnimationContainer* s_Container;
		static std::map<Uint32, AnimationComplexType*>* s_AnimationMap;
		using animmap_iter = std::map<Uint32, AnimationComplexType*>::iterator;

	public:
		AnimationContainer(AnimationContainer&) = delete;
		~AnimationContainer();

	public:
		static AnimationComplexType* GetAnimationComplexType(Uint32 key);
		static Animation* GetAnimationReference(Uint32 key);
		static bool Contains(const GameObject2D& go2d);
		static void Insert(Animation& animation, Uint32 key, Uint32 numOfAnimations, const GameObject2D& go2d);
		static void Erase(Uint32 key);
		static void Delete(void);

	private:
		AnimationContainer();

	};

}

