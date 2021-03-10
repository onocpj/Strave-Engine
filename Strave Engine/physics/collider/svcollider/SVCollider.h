#pragma once

#include "ColliderMesh.h"
#include "../../../entities/RigidBody2D.h"
#include "../../../core/datatypes/DT.h"


#define SVC_MAX_MESHES		10
#define SVC_TERMINAL_MESH	1
#define SVC_IN_COLLISION	1
#define SVC_FALSE			UNDEF_BOOL


namespace Strave 
{
	class SV_SANDBOX_API Collider
	{
	public:
		struct nom_t
		{
			Uint16 colA, colB;
		}; // Number of Meshes for each collider
		typedef bool(*mot_fptr)(const ColliderMesh::Mesh&, const ColliderMesh::Mesh&); // Mesh Overlap Test - function pointer

		~Collider();
		static Collider* Create(const ColliderMesh::Mesh mesh[SVC_MAX_MESHES], Uint16 numberOfMeshes);
		void Init(const ColliderMesh::Mesh mesh[SVC_MAX_MESHES]);

		inline ColliderMesh::Mesh* GetMesh(void) const { return m_Mesh; }
		inline Uint16 GetMeshCount(void) const { return m_NumberOfMeshes; }
		inline void Show(void) { m_ShowCollider = true; }
		inline void Hide(void) { m_ShowCollider = false; }
		inline void Enable(void) { m_ColliderEnabled = true; }
		inline void Disable(void) { m_ColliderEnabled = false; }
		inline bool InCollision(void) const { return m_InCollision; }

		static void CollisionDetection(void);
		static void OnTriggerCollision(Collider& collider);

	private:
		Collider(const ColliderMesh::Mesh mesh[SVC_MAX_MESHES], Uint16 numberOfMeshes);
		void InitMesh(const ColliderMesh::Mesh mesh[SVC_MAX_MESHES]);
		static mot_fptr GetMOT(ColliderMesh::Type type);

		friend void RigidBody2D::EnableCollider(void);
		friend void RigidBody2D::DisableCollider(void);
		friend void RigidBody2D::ShowCollider(void);
		friend void RigidBody2D::HideCollider(void);

		static bool ColliderMeshTest(const Collider& col1, const Collider& col2, nom_t numberOfMeshes);
		static bool BoxOverlapTest(const ColliderMesh::Mesh& box1, const ColliderMesh::Mesh& box2);
		static bool CircleOverlapTest(const ColliderMesh::Mesh& circ2, const ColliderMesh::Mesh& circ1);
		static bool MeshOverlapTest(const ColliderMesh::Mesh& mesh1, const ColliderMesh::Mesh& mesh2);

		inline void UpdateCollisionState(sv_state_t state) { m_InCollision = state; }

	protected:
		sv_state_t				m_ColliderEnabled;

	private:
		Uint64					m_Indetifier;
		ColliderMesh::Type		m_MeshType;
		ColliderMesh::Mesh*		m_Mesh;
		Uint16					m_NumberOfMeshes;
		sv_state_t				m_InCollision;
		sv_state_t				m_ShowCollider;
	};
}

