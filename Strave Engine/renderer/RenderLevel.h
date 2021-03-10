#pragma once

#include<vector>

namespace Strave 
{
	namespace nRenderer { class DistanceRenderer; }

	class GameObject2D;
	class RenderLevel
	{
	private:
		friend nRenderer::DistanceRenderer;

	protected:
		static const void SortObjectsByLevel(const std::vector<GameObject2D*>& objects);
	};
}

