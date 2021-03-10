#include "BoundingBox.h"

#include <SFML/Graphics.hpp>

#include "../../../../core/datatypes/Convert.h"


namespace Strave
{
	//=============================================================================
	//								class: BoundingBox
	//=============================================================================
	BoundingBox::BoundingBox(const ColliderMesh::Mesh& mesh)
	{
		Init(mesh);
	}

	void BoundingBox::Init(const ColliderMesh::Mesh& mesh)
	{
		sf::Transform trans = mesh.getTransform();
		sf::IntRect local = mesh.getTextureRect();

		m_Points[0] = Convert::ToVector2f(trans.transformPoint(0.f, 0.f));
		m_Points[1] = Convert::ToVector2f(trans.transformPoint((float)local.width, 0.f));
		m_Points[2] = Convert::ToVector2f(trans.transformPoint((float)local.width, (float)local.height));
		m_Points[3] = Convert::ToVector2f(trans.transformPoint(0.f, (float)local.height));
	}

	void BoundingBox::ProjectOntoAxis(const Vector2f& Axis, float& Min, float& Max)
	{
		Min = (m_Points[0].x * Axis.x + m_Points[0].y * Axis.y);
		Max = Min;

		for (int j = 1; j < 4; j++) {
			float projection = (m_Points[j].x * Axis.x + m_Points[j].y * Axis.y);

			if (projection < Min) 
				Min = projection;

			if (projection > Max)
				Max = projection;
		}
	}
}
