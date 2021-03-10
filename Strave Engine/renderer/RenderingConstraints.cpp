#include "RenderingConstraints.h"

#include "../core/datatypes/UDT.h"


namespace Strave {

	namespace nRenderer {

		RenderingConstraints::RenderingConstraints()
			: m_InitPoint(UNDEF_VECTOR2F), m_RenderRadius(UNDEF_FLOAT), m_PreloadRadius(UNDEF_FLOAT)
		{}
		RenderingConstraints::RenderingConstraints(const Vector2f& initPoint, float renderingRadius, float preloadRadius)
			: m_InitPoint(initPoint), m_RenderRadius(VerifyRenderingRadius(renderingRadius)), m_PreloadRadius(preloadRadius)
		{}
		RenderingConstraints::~RenderingConstraints() {}

	}

}

