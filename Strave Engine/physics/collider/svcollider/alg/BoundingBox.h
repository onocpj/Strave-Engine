#pragma once

#include "../../../../core/datatypes/Vector2.h"
#include "../ColliderMesh.h"


namespace Strave
{
	class BoundingBox
	{
	public:
		BoundingBox() = default;
		BoundingBox(const ColliderMesh::Mesh& mesh);
		~BoundingBox() = default;

		void ProjectOntoAxis(const Vector2f& Axis, float& Min, float& Max);

	private:
		void Init(const ColliderMesh::Mesh& mesh);

	public:
		Vector2f m_Points[4];
	};
}

