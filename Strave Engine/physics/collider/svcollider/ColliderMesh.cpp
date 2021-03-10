#include "ColliderMesh.h"

#include "../../../core/datatypes/Convert.h"

namespace Strave
{
	//=============================================================================
	//								class: BasicMesh
	//=============================================================================
	ColliderMesh::BasicMesh::BasicMesh() :
		sf::Sprite(),
		m_Constraints(SV_DEF_COLLIDERMESH_BASIC)
	{
		
	}

	ColliderMesh::BasicMesh::BasicMesh(const ColliderConstraints& constrains) :
		sf::Sprite(),
		m_Constraints(constrains)
	{
		// !! INITIALIZE HERE SPRITE THAT REPRESENTS COLLIDER !!
	}

	bool ColliderMesh::MeshTypesEquivalent(const Type& t1, const Type& t2)
	{
		return t1 == t2 ? true : false;
	}
	
	Vector2f ColliderMesh::BasicMesh::GetMeshCenter(void) const
	{
		sf::FloatRect AABB = this->getGlobalBounds();

		return Vector2f(AABB.left + AABB.width / 2.f, AABB.top + AABB.height / 2.f);
	}

	Vector2f ColliderMesh::BasicMesh::GetMeshSize(void) const
	{
		sf::IntRect originalSize = this->getTextureRect();
		Vector2f scale = Convert::ToVector2f(this->getScale());

		return Vector2f(originalSize.width * scale.x, originalSize.height * scale.y);
	}

	//=============================================================================
	//								class: Rectangle
	//=============================================================================
	ColliderMesh::Box::Box(const Constraints& constraints) : 
		BasicMesh({ constraints.alignment, constraints.position, constraints.rotation }), 
		m_Constraints(constraints)
	{

	}

	ColliderMesh::Box* ColliderMesh::Box::Create(const Constraints& constraints)
	{
		return new ColliderMesh::Box(constraints);
	}

	//=============================================================================
	//								class: Circle
	//=============================================================================

	ColliderMesh::Circle::Circle(const Constraints& constraints) :
		BasicMesh({ constraints.alignment, constraints.position, constraints.rotation }),
		m_Constraints(constraints)
	{

	}
}