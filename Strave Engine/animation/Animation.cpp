#include "Animation.h"

#include "../window/WindowsWindow.h"
#include "../core/datatypes/Convert.h"
#include "../entities/GameObject2D.h"
#include "../animation/AnimationContainer.h"
#include "../core/thread/Thread.h"
#include "../core/additive/Additive.h"

namespace Strave {

	//=============================================================================
	//								class: Animation
	//=============================================================================
	Uint32 Animation::AnimationKey::s_Key = UNDEF_UINT;

	Animation::Animation(const sf::Texture& texture, Uint16 columns, float animationTime)
		: m_Constraints(UNDEF_PTR), m_AnimationContainerReference(UNDEF_PTR), m_AnimationTexture(&texture),
		m_GameObjectReference(UNDEF_PTR), m_ImageCount(UNDEF_VECTOR2U),
		m_AnimationKey(UNDEF_INT32), m_CurrentImageCount(UNDEF_INT32),
		m_NumberOfAnimations(UNDEF_INT32), m_AnimationElapsedTime(UNDEF_FLOAT),
		m_AnimationActive(UNDEF_PTR), m_UseDefaultAnimation(true)
	{
		Init(texture, columns, animationTime);
	}

	Animation::Animation(const sf::Texture& texture, Uint16 rows, const Constraints constraints[])
		: m_Constraints(UNDEF_PTR),
		m_AnimationContainerReference(UNDEF_PTR), m_AnimationTexture(&texture),
		m_GameObjectReference(UNDEF_PTR), m_ImageCount(UNDEF_VECTOR2U),
		m_AnimationKey(UNDEF_INT32), m_CurrentImageCount(UNDEF_INT32),
		m_NumberOfAnimations(UNDEF_INT32), m_AnimationElapsedTime(UNDEF_FLOAT),
		m_AnimationActive(UNDEF_PTR), m_UseDefaultAnimation(true)
	{
		Init(texture, rows, constraints);
	}

	Animation::~Animation() {
		delete m_AnimationActive;
		delete[] m_Constraints;
	}

	Animation* Animation::Create(const sf::Texture& texture, Uint16 columns, float animationTime) {
		return new Animation(texture, columns, animationTime);
	}

	Animation* Animation::Create(const sf::Texture& texture, const Uint16 rows, const Constraints constraints[]) {
		return new Animation(texture, rows, constraints);
	}

	void Animation::Init(const sf::Texture& texture, Uint16 columns, float animationTime) {
		m_ImageCount = Vector2u(columns, 1);

		Vector2u t_AnimationTextureSize = Convert::ToVector2u(texture.getSize());
		Vector2u t_CorrectImageCount = Animation::CorrectImageCount(m_ImageCount);

		m_AnimatedFrame.width = t_AnimationTextureSize.x / t_CorrectImageCount.x;
		m_AnimatedFrame.height = t_AnimationTextureSize.y / t_CorrectImageCount.y;
		m_AnimationSwitchTime = (animationTime / t_CorrectImageCount.x);
		m_NumberOfAnimations = t_CorrectImageCount.y;
	}

	void Animation::Init(const sf::Texture& texture, Uint16 rows, const Constraints constraints[]) {
		m_Constraints = new Constraints[rows];

		if (Additive::Array::CopyArray(constraints, m_Constraints, rows) == ERROR_TYPE)
			m_ImageCount = Vector2u(UNDEF_UINT, rows);
		else
			m_ImageCount = Vector2u((Uint32)FindLargestColumnNumber(constraints, rows), rows);

		Vector2u t_AnimationTextureSize = Convert::ToVector2u(texture.getSize());
		Vector2u t_CorrectImageCount = Animation::CorrectImageCount(m_ImageCount);

		m_AnimatedFrame.width = t_AnimationTextureSize.x / t_CorrectImageCount.x;
		m_AnimatedFrame.height = t_AnimationTextureSize.y / t_CorrectImageCount.y;
		m_AnimationSwitchTime = (m_Constraints[SV_DEF_ANIMATION].animationTime / t_CorrectImageCount.x);
		m_NumberOfAnimations = t_CorrectImageCount.y;
	}

	Uint16 Animation::FindLargestColumnNumber(const Constraints*& constraints, Uint16 rows) {
		Uint16 t_Largest = constraints[0].columns;

		for (Uint64 index = 0; index != rows; index++) {
			if (constraints[index].columns > t_Largest) {
				t_Largest = constraints[index].columns;
			}
		}

		return t_Largest;
	}

	void Animation::LoadConstraints(const Constraints& constraints) {
		m_ImageCount.x = constraints.columns;
		m_AnimationSwitchTime = (constraints.animationTime / constraints.columns);
	}

	Vector2u Animation::CorrectImageCount(const Vector2u& imageCount) {
		Vector2u t_ImageCount = imageCount;

		if (t_ImageCount.x < 1) {
			t_ImageCount.x = 1;
		}
		if (t_ImageCount.y < 1) {
			t_ImageCount.y = 1;
		}

		return t_ImageCount;
	}

	void Animation::Update(GameObject2D& go2d) {
		Uint32 t_ObjectsAnimationKey = Animation::GetObjectAnimationKey(go2d);

		if (t_ObjectsAnimationKey != NOT_ASSIGNED) {
			AnimationContainer::GetAnimationReference(t_ObjectsAnimationKey)->Update();
		}
	}

	void Animation::Update(void) {
		m_AnimationElapsedTime += EngineClocks::deltaTime;

		if (m_AnimationElapsedTime >= m_AnimationSwitchTime) {
			m_AnimationElapsedTime -= m_AnimationSwitchTime;
			m_CurrentImageCount++;

			if (m_CurrentImageCount >= m_ImageCount.x) {
				m_AnimationElapsedTime = 0.0f;
				m_CurrentImageCount = 0;

				if (!m_UseDefaultAnimation) {
					if (m_Constraints != UNDEF_PTR) {
						Animation::LoadConstraints(m_Constraints[SV_DEF_ANIMATION]);
					}

					m_AnimationContainerReference->AssignDefaultAnimation();
					AnimationFlag::Fold(m_AnimationActive);
				}
			}
		}

		m_AnimatedFrame.left = m_AnimatedFrame.width * m_CurrentImageCount;
		m_GameObjectReference->GetModel<sf::Sprite>()->setTextureRect(m_AnimatedFrame);
	}

	void Animation::Animate(Uint16 animation) {
		if (animation <= m_NumberOfAnimations - 1) {
			if (!m_AnimationContainerReference->HaveActiveAnimation()) {
				if (!AnimationFlag::RisedState(m_AnimationActive)) {
					if (m_Constraints != UNDEF_PTR) {
						Animation::LoadConstraints(m_Constraints[animation]);
					}

					m_AnimationContainerReference->SetActiveAnimationTo(animation);
					m_AnimationActive = AnimationFlag::Rise(*this);
				}
			}
		}
	}

	void Animation::AssignTo(GameObject2D& go2d) {
		m_GameObjectReference = &go2d;
		m_AnimationKey = AnimationKey::AssignKey(&go2d);
		m_GameObjectReference->m_Key.animationKey = m_AnimationKey;

		go2d.UpdateTexture(*m_AnimationTexture);

		if (m_AnimationKey != UNDEFINED_ANIMATION_KEY) {
			if (m_Constraints != UNDEF_PTR) {
				Animation::LoadConstraints(m_Constraints[SV_DEF_ANIMATION]);
			}

			AnimationContainer::Insert(*this, m_AnimationKey, m_NumberOfAnimations, go2d);
			m_AnimationContainerReference = AnimationContainer::GetAnimationComplexType(m_AnimationKey);

		}
		else {
			SV_CORE_INFO("function Animation::AssignTo(GameObject2D&): Object have alaready assigned animation");
		}
	}

	void Animation::Stop(void) {
		if (m_AnimationActive != NULL_PTR) {
			if (m_Constraints != UNDEF_PTR) {
				Animation::LoadConstraints(m_Constraints[SV_DEF_ANIMATION]);
			}

			m_AnimationContainerReference->AssignDefaultAnimation();
			AnimationFlag::Fold(m_AnimationActive);
		}
	}

	bool Animation::IsActive(Uint16 animation) {
		return (Uint16)m_AnimationContainerReference->GetActiveAnimation() == animation ? true : false;
	}

	Int32 Animation::GetObjectAnimationKey(const GameObject2D& go2d) {
		return go2d.m_Key.animationKey;
	}

	//=============================================================================
	//								class: AnimationKey
	//=============================================================================

	int Animation::AnimationKey::AssignKey(const GameObject2D* go2d) {
		if (!AnimationContainer::Contains(*go2d)) {
			Uint32 t_NewAssignedKey = s_Key;
			s_Key++;

			return t_NewAssignedKey;
		}

		return UNDEFINED_ANIMATION_KEY;
	}

	//=============================================================================
	//								class: AnimationFlag
	//=============================================================================

	Animation::AnimationFlag::AnimationFlag(Animation& animation)
		: m_Animation(&animation)
	{
		animation.m_UseDefaultAnimation = false;
		animation.m_AnimatedFrame.top = animation.m_AnimatedFrame.height * (animation.m_AnimationContainerReference->GetActiveAnimation());
	}

	Animation::AnimationFlag::~AnimationFlag() {
		m_Animation->m_UseDefaultAnimation = true;
		m_Animation->m_AnimatedFrame.top = START_POSITION;
	}

	void Animation::AnimationFlag::Fold(AnimationFlag*& flag) {
		thread::ThreadPool::SendToMutex([&flag]() {
			delete flag;
			flag = NULL_PTR;
		});
	}

}

