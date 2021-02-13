#pragma once

#include "Vector2.h"


namespace Strave {

	namespace NRenderer {

		class RenderingConstraints {

		public:
			RenderingConstraints();
			RenderingConstraints(const Vector2f& initPoint, float renderingRadius, float preloadRadius);
			~RenderingConstraints();

			inline void SetInitPoint(const Vector2f& initPoint) { m_InitPoint = initPoint; }
			inline void SetRenderingRadius(float radius) { m_RenderRadius = VerifyRenderingRadius(radius); }
			inline void SetPreloadRadius(float radius) { m_PreloadRadius = radius; }
			inline const Vector2f& GetPoint(void) const { return m_InitPoint; }
			inline const float& GetRenderRadius(void) const { return m_RenderRadius; }
			inline const float& GetPreloadedRadius(void) const { return m_PreloadRadius; }

		private:
			inline float VerifyRenderingRadius(float renderingRadius) { return renderingRadius <= m_PreloadRadius ? renderingRadius : m_PreloadRadius; }


			Vector2f	m_InitPoint;
			float		m_RenderRadius;
			float		m_PreloadRadius;

		};

	}

}
