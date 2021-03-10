#include "SVCollider.h"

#include "ColliderContainer.h"
#include "alg/BoundingBox.h"

#include "../../../core/datatypes/UDT.h"
#include "../../../core/additive/Additive.h"
#include "../../../core/logger/Log.h"
#include "../../../core/datatypes/Convert.h"


namespace Strave 
{
	Collider::Collider(const ColliderMesh::Mesh mesh[SVC_MAX_MESHES], Uint16 numberOfMeshes)
		: m_Indetifier(BC_UNDEF_KEY), m_MeshType(mesh[0].GetMeshType()), 
		m_Mesh(UNDEF_PTR), m_NumberOfMeshes(numberOfMeshes),
		m_ColliderEnabled(SVC_FALSE), m_ShowCollider(SVC_FALSE), m_InCollision(SVC_FALSE)
	{
		Init(mesh);
	}
	Collider::~Collider()
	{
		delete[] m_Mesh;
	}

	Collider* Collider::Create(const ColliderMesh::Mesh mesh[SVC_MAX_MESHES], Uint16 numberOfMeshes)
	{
		return new Collider(mesh, numberOfMeshes);
	}

	void Collider::Init(const ColliderMesh::Mesh mesh[SVC_MAX_MESHES])
	{
		m_Indetifier = ColliderContainer::Get().InsertToContainer(*this);
		m_Mesh = new ColliderMesh::Mesh[SVC_MAX_MESHES]();

		InitMesh(mesh);
	}

	void Collider::CollisionDetection(void)
	{
		Collider* CA = UNDEF_PTR; // Pointer to Collider A
		Collider* CB = UNDEF_PTR; // Pointer to Collider B
		Int64 CCSize = ColliderContainer::Get().GetSize(); // Collider Container Size

		// We need to check each collider A with collider B for collision
		for (Int64 keyA = 0; keyA != CCSize; keyA++) 
		{
			CA = ColliderContainer::Get().GetElement(keyA);

			for (Int64 keyB = 0; keyB != CCSize; keyB++) 
				if (keyA != keyB) // Make sure that collider will not apply collision detection for himself
				{
					CB = ColliderContainer::Get().GetElement(keyB);

					// Test colliders for collision
					if (Collider::ColliderMeshTest(*CA, *CB, { CA->m_NumberOfMeshes, CB->m_NumberOfMeshes }) == true) 
					{
						CA->UpdateCollisionState(SVC_IN_COLLISION);
						CB->UpdateCollisionState(SVC_IN_COLLISION);

						break;
					}
					else
					{
						if (CA->InCollision() == true)
							CA->UpdateCollisionState(!SVC_IN_COLLISION);

						if (CB->InCollision() == true)
							CB->UpdateCollisionState(!SVC_IN_COLLISION);
					}
				}
		}
	}

	void Collider::OnTriggerCollision(Collider& collider)
	{
		Collider* CA = &collider;
		Collider* CB = UNDEF_PTR;
		Int64 CCSize = ColliderContainer::Get().GetSize();

		for (Int64 key = 0; key != CCSize; key++)
		{
			CB = ColliderContainer::Get().GetElement(key);
			if (CA->m_Indetifier != key)
				if (Collider::ColliderMeshTest(*CA, *CB, { CA->m_NumberOfMeshes, CB->m_NumberOfMeshes }) == true) 
				{
					CA->UpdateCollisionState(SVC_IN_COLLISION);
					break;
				}
				else 
				{
					if (CA->InCollision() == true)
						CA->UpdateCollisionState(!SVC_IN_COLLISION);
				}
		}
	}

	void Collider::InitMesh(const ColliderMesh::Mesh mesh[SVC_MAX_MESHES])
	{
		static int (*copyMesh)(const ColliderMesh::Mesh*& from, ColliderMesh::Mesh*& to, Uint64 size) = UNDEF_PTR;
		int meshCopied = UNDEF_INT;

		copyMesh = &Additive::Array::CopyArray;
		meshCopied = copyMesh(mesh, m_Mesh, m_NumberOfMeshes);

		if (meshCopied != ERROR_TYPE)
			SV_CORE_TRACE("function void Collider::Init(const ColliderMesh::Type mesh[]): mesh was copied");
		else 
			SV_CORE_ERROR("function void Collider::Init(const ColliderMesh::Type mesh[]): unable to copy mesh");
	}

	Collider::mot_fptr Collider::GetMOT(ColliderMesh::Type type)
	{
		switch (type)
		{
		case ColliderMesh::Type::Box: return &Collider::BoxOverlapTest;
		case ColliderMesh::Type::Circle: return &Collider::CircleOverlapTest;
		default: return UNDEF_PTR;
		}
	}

	bool Collider::ColliderMeshTest(const Collider& col1, const Collider& col2, nom_t numberOfMeshes)
	{
		ColliderMesh::Type col1MT = col1.m_MeshType;
		bool meshesOverlap = UNDEF_BOOL;

		// Chceck if both mesh types are same. Doing this to know which function to call.
		// Instead of calling heavy function to calculate collision between two different 
		// meshes, we can use function that calculates collision between two same meshes
		// like rectangle or circle which are lighter.
		if (ColliderMesh::MeshTypesEquivalent(col1MT, col2.m_MeshType))
		{
			// Get Mesh Overlap function pointer that match mesh type
			auto MOTFunction = Collider::GetMOT(col1MT);

			// We need to check each mesh of collider A with meshes of collider B for overlaping
			for(Uint16 i = 0; i != numberOfMeshes.colA; i++)
				for (Uint16 j = 0; j != numberOfMeshes.colB; j++)
				{
					// Calling function based on mesh type to chceck mesh overlaping.
					// Function returns boolean value that represents if meshes are overlaping
					meshesOverlap = MOTFunction((ColliderMesh::Mesh)col1.m_Mesh[i], (ColliderMesh::Mesh)col2.m_Mesh[j]);

					if (meshesOverlap == true) 
						return meshesOverlap;
				}

		}  else  
		{
			for (Uint16 i = 0; i != numberOfMeshes.colA; i++)
				for (Uint16 j = 0; j != numberOfMeshes.colB; j++)
				{
					// Calling function based on mesh type to chceck mesh overlaping.
					// Function returns boolean value that represents if meshes are overlaping
					Collider::MeshOverlapTest((ColliderMesh::Mesh)col1.m_Mesh[i], (ColliderMesh::Mesh)col2.m_Mesh[j]);

					if (meshesOverlap == true)
						return meshesOverlap;
				}
		}

		return meshesOverlap;
	}

	bool Collider::BoxOverlapTest(const ColliderMesh::Mesh& box1, const ColliderMesh::Mesh& box2)
	{
		// BBB - stands for: Box Bounding Box
		BoundingBox BBB1(box1);
		BoundingBox BBB2(box2);

		Vector2f axes[4] = 
		{
			Vector2f(BBB1.m_Points[1].x - BBB1.m_Points[0].x,
			BBB1.m_Points[1].y - BBB1.m_Points[0].y),
			Vector2f(BBB1.m_Points[1].x - BBB1.m_Points[2].x,
			BBB1.m_Points[1].y - BBB1.m_Points[2].y),
			Vector2f(BBB2.m_Points[0].x - BBB2.m_Points[3].x,
			BBB2.m_Points[0].y - BBB2.m_Points[3].y),
			Vector2f(BBB2.m_Points[0].x - BBB2.m_Points[1].x,
			BBB2.m_Points[0].y - BBB2.m_Points[1].y)
		};

		for (int i = 0; i < 4; i++) 
		{
			float minRBB1, maxRBB1, minRBB2, maxRBB2;

			BBB1.ProjectOntoAxis(axes[i], minRBB1, maxRBB1);
			BBB2.ProjectOntoAxis(axes[i], minRBB2, maxRBB2);

			if (!((minRBB2 <= maxRBB1) && (maxRBB2 >= minRBB1)))
				return false;
		}

		return true;
	}

	bool Collider::CircleOverlapTest(const ColliderMesh::Mesh& circ1, const ColliderMesh::Mesh& circ2)
	{
		Vector2f obj1Size = circ1.GetMeshSize();
		Vector2f obj2Size = circ2.GetMeshSize();
		float radius1 = (obj1Size.x + obj1Size.y) / 4;
		float radius2 = (obj2Size.x + obj2Size.y) / 4;

		Vector2f distance = circ1.GetMeshCenter() - circ2.GetMeshCenter();

		return (distance.x * distance.x + distance.y * distance.y <= (radius1 + radius2) * (radius1 + radius2));
	}

	bool Collider::MeshOverlapTest(const ColliderMesh::Mesh& mesh1, const ColliderMesh::Mesh& mesh2)
	{
		return 0;
	}
}
