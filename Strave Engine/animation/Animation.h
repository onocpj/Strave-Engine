#pragma once

#include <SFML/Graphics.hpp>

#include "../core/datatypes/UDT.h"
#include "../core/datatypes/DT.h"
#include "../core/datatypes/Vector4.h"
#include "../core/logger/Log.h"


#define START_POSITION					UNDEF_UINT
#define NOT_ASSIGNED					ERROR_TYPE
#define UNDEFINED_ANIMATION_KEY			ERROR_TYPE
#define SV_DEF_ANIMATION_TIME			UNDEF_FLOAT
#define SV_DEF_ANIMATION				UNDEF_UINT
#define SV_DEF_ANIMATION_CONSTRAINTS	{ SV_DEF_ANIMATION, SV_DEF_ANIMATION_TIME }


namespace Strave 
{
	class Texture;
	class GameObject2D;
	class AnimationComplexType;
	class SV_SANDBOX_API Animation
	{
	private:
		class AnimationFlag;

		friend AnimationFlag;
		friend DistanceRenderer;
		friend Renderer;
		friend GameObject2D;

		class AnimationKey 
		{
		public:
			static int AssignKey(const GameObject2D* go2d);

		private:
			static Uint32 s_Key;
		};
		class AnimationFlag 
		{
		public:
			AnimationFlag(Animation& animation);
			~AnimationFlag();

			inline static bool RisedState(AnimationFlag* flag) 
			{
				return (flag != UNDEF_PTR ? true : false);
			}
			inline static AnimationFlag* Rise(Animation& animation) 
			{
				return new AnimationFlag(animation);
			}
			static void Fold(AnimationFlag*& flag);

		private:
			Animation* m_Animation;

		};

	public:
		///
		/// \brief Represents number of columns and animation time to loop
		/// through those columns. Can be used to customize your animation grid 
		/// texture layout.
		/// 
		/// \param Body: { columns, animationTime }
		/// \param
		/// \param columns - Number of columns in one row (number of animation states in one row)
		/// \param animationTime - Time to loop through all columns (in seconds)
		///
		struct Constraints
		{
			Uint16	columns;
			float	animationTime;
		};

		~Animation();

		///
		/// \brief Creates animation
		///
		/// \param texture - animation texture
		/// \param imageCount - number of pictures animation contains
		/// \param animationTime - animation time
		///
		/// \return Pointer to created animation
		/// 
		static Animation* Create(const sf::Texture& texture, Uint16 columns, float animationTime);
		static Animation* Create(const sf::Texture& texture, const Uint16 rows, const Constraints constraints[]);
		///
		/// \brief Starts animation
		///
		void Animate(Uint16 animation = (Uint16)SV_DEF_ANIMATION);
		///
		/// \brief Assign animation to object
		///
		void AssignTo(GameObject2D& go2d);
		///
		/// \brief Stop animation
		///
		void Stop(void);
		///
		/// \brief Chceck if animation is running
		///
		/// \return True if animation is active, false otherwise
		///
		bool IsActive(Uint16 animation);

	private:
		Animation(const sf::Texture& texture, Uint16 columns, float animationTime);
		Animation(const sf::Texture& texture, Uint16 rows, const Constraints constraints[]);

		void Init(const sf::Texture& texture, Uint16 columns, float animationTime);
		void Init(const sf::Texture& texture, Uint16 rows, const Constraints constraints[]);

		Uint16 FindLargestColumnNumber(const Constraints*& constraints, Uint16 rows);
		Vector2u CorrectImageCount(const Vector2u& imageCount);
		void LoadConstraints(const Constraints& constraints);
		void Update(void);

	protected:
		static Int32 GetObjectAnimationKey(const GameObject2D& go2d);
		static void Update(GameObject2D& go2d);

	protected:
		AnimationFlag*			m_AnimationActive;
		bool					m_UseDefaultAnimation;

	private:
		Constraints*			m_Constraints;
		AnimationComplexType*	m_AnimationContainerReference;
		GameObject2D*			m_GameObjectReference;
		const sf::Texture*		m_AnimationTexture;
		sf::IntRect				m_AnimatedFrame;
		Vector2u				m_ImageCount;
		Uint32					m_AnimationKey;
		Uint32					m_CurrentImageCount;
		Uint32					m_NumberOfAnimations;
		float					m_AnimationElapsedTime;
		float					m_AnimationSwitchTime;
	};

	typedef Texture AnimationTexture;
}

