#include "AnimationContainer.h"
#include "Animation.h"

#include "../entities/GameObject2D.h"
#include "../core/logger/Log.h"
#include "../core/thread/Thread.h"

#define NOT_ACTIVE_ANIMATION -1

namespace Strave {

	//=============================================================================
	//							class: AnimationContainer
	//=============================================================================

	AnimationContainer* AnimationContainer::s_Container = new AnimationContainer();
	std::map<Uint32, AnimationComplexType*>* AnimationContainer::s_AnimationMap = new std::map<Uint32, AnimationComplexType*>();

	AnimationContainer::AnimationContainer() {}
	AnimationContainer::~AnimationContainer() {
		delete s_AnimationMap;
	}

	AnimationComplexType* AnimationContainer::GetAnimationComplexType(Uint32 key) {
		try {
			return s_AnimationMap->at(key);

		}
		catch (...) {
			SV_CORE_INFO("function AnimationContainer::GetAnimationComplexType(Int32): Element not found");
		}

		return UNDEF_PTR;
	}
	Animation* AnimationContainer::GetAnimationReference(Uint32 key) {
		try {
			return s_AnimationMap->at(key)->m_AnimationReference;

		} catch (...) {
			SV_CORE_INFO("function AnimationContainer::GetAnimationReference(Int32): Element not found");
		}

		return UNDEF_PTR;
	}
	bool AnimationContainer::Contains(const GameObject2D& go2d) {
		if (s_AnimationMap->size() > 0) {
			for (animmap_iter iter = s_AnimationMap->begin(), end = s_AnimationMap->end(); iter != end; ++iter) {
				if (iter->second->m_ObjectReference == &go2d) {
					return true;
				}
			}
		}

		return false;
	}
	void AnimationContainer::Insert(Animation& animation, Uint32 key, Uint32 numOfAnimations, const GameObject2D& go2d) {
		AnimationComplexType* t_ComplexType = new AnimationComplexType();

		t_ComplexType->m_AnimationReference = &animation;
		t_ComplexType->m_ObjectReference = &go2d;
		t_ComplexType->m_Animations = new Uint32[numOfAnimations];

		s_AnimationMap->insert(std::pair<Uint32, AnimationComplexType*>(key, t_ComplexType));
	}
	void AnimationContainer::Erase(Uint32 key) {
		if (s_AnimationMap->size() > 0) {

			auto animation = s_AnimationMap->find(key);
			delete animation->second;

			s_AnimationMap->erase(animation);

		} else {
			SV_CORE_INFO("function AnimationContainer::Erase(Int32): Container is empty");
		}
	}
	void AnimationContainer::Delete(void) {
		delete s_Container;
	}

	//=============================================================================
	//					class: AnimationContainer::ComplexType
	//=============================================================================

	AnimationComplexType::AnimationComplexType() :
		m_AnimationReference(UNDEF_PTR), m_ObjectReference(UNDEF_PTR), 
		m_Animations(UNDEF_PTR), m_ActiveAnimation(SV_DEF_ANIMATION)
	{}
	AnimationComplexType::~AnimationComplexType() {
		delete[] m_Animations;
	}

	void AnimationComplexType::SetActiveAnimationTo(Uint32 animation) {
		this->m_ActiveAnimation = animation;
	}
	void AnimationComplexType::AssignDefaultAnimation() {
		this->m_ActiveAnimation = SV_DEF_ANIMATION;
	}
	bool AnimationComplexType::HaveActiveAnimation() const {
		return this->m_ActiveAnimation > SV_DEF_ANIMATION ? true : false;
	}

}
