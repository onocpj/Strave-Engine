#pragma once

#include "../../../core/datatypes/Vector2.h"
#include "../../../core/datatypes/UDT.h"
#include "../../../core/logger/Log.h"

#include <SFML/Graphics.hpp>


#define CM_UNDEF_UINT 0xffffffff
#define CM_UNDEF_MESH 0

// Basic Mesh
#define SV_DEF_COLLIDERMESH_POSITION			{ UNDEF_FLOAT, UNDEF_FLOAT }
#define SV_DEF_COLLIDERMESH_ALIGNMENT			{ Strave::ColliderMesh::BasicMesh::Alignment::Horizontal::Undefined, Strave::ColliderMesh::BasicMesh::Alignment::Vertical::Undefined }
#define SV_DEF_COLLIDERMESH_ROTATION			0.0f
#define SV_DEF_COLLIDERMESH_BASIC				{ SV_DEF_COLLIDERMESH_ALIGNMENT, SV_DEF_COLLIDERMESH_POSITION, SV_DEF_COLLIDERMESH_ROTATION }	
// Rectangle Mesh
#define SV_DEF_COLLIDERMESH_BOX_SIZE			{ 1.0f, 1.0f }
#define SV_DEF_COLLIDERMESH_BOX					{ SV_DEF_COLLIDERMESH_ALIGNMENT, SV_DEF_COLLIDERMESH_POSITION, SV_DEF_COLLIDERMESH_ROTATION, SV_DEF_COLLIDERMESH_BOX_SIZE }	
//Circle Mesh
#define SV_DEF_COLLIDERMESH_CIRCLE_RADIUS		UNDEF_FLOAT
#define SV_DEF_COLLIDERMESH_CIRCLE				{ SV_DEF_COLLIDERMESH_ALIGNMENT, SV_DEF_COLLIDERMESH_POSITION, SV_DEF_COLLIDERMESH_ROTATION, SV_DEF_COLLIDERMESH_CIRCLE_RADIUS }	


namespace Strave 
{
	class Collider;
	class SV_SANDBOX_API ColliderMesh abstract
	{
	public:
		enum class Type
		{
			Undefined = 0,
			Box,
			Circle
		};

		typedef class BasicMesh : public sf::Sprite
		{
		public:
			struct Alignment {
				enum class Horizontal
				{
					Undefined = 0,
					Left,
					Right,
					Center
				};
				enum class Vertical
				{
					Undefined = 0,
					Top,
					Bottom,
					Center
				};

				Horizontal	horizontal;
				Vertical	vertical;
			};
			struct ColliderConstraints
			{
				ColliderConstraints(Alignment alignment, Vector2f position, float rotation) :
					alignment(alignment), position(position), rotation(rotation)
				{}
				virtual ~ColliderConstraints() = default;

				Alignment alignment = SV_DEF_COLLIDERMESH_ALIGNMENT;
				Vector2f position = SV_DEF_COLLIDERMESH_POSITION;
				float rotation = SV_DEF_COLLIDERMESH_ROTATION;
			};

			BasicMesh();
			BasicMesh(const ColliderConstraints& constrains);
			virtual ~BasicMesh() = default;

			Vector2f GetMeshCenter(void) const;
			Vector2f GetMeshSize(void) const;
			inline void SetAlignment(const Alignment& alignment) { m_Constraints.alignment = alignment; }
			inline void SetPosition(const Vector2f& position) { m_Constraints.position = position; }
			inline void SetRotation(float degrees) { m_Constraints.rotation = degrees; }
			inline const Alignment& GetAlignment(void) const { return m_Constraints.alignment; }
			inline const Vector2f& GetPosition(void) const { return m_Constraints.position; }
			inline float GetRotation(void) const { return m_Constraints.rotation; }
			virtual inline Type GetMeshType(void) const { return static_cast<Type>(CM_UNDEF_MESH); }

		private:
			ColliderConstraints m_Constraints;

		} Mesh;
		class Box : public BasicMesh
		{
		public:
			struct Constraints : public BasicMesh::ColliderConstraints
			{
				Constraints(Alignment alignment, Vector2f position, float rotation, Vector2u size) :
					BasicMesh::ColliderConstraints(alignment, position, rotation),
					size(size)
				{}
				~Constraints() = default;

				Vector2u size = SV_DEF_COLLIDERMESH_BOX_SIZE;
			};

			static Box* Create(const Constraints& constraints);
			inline void SetSize(const Vector2u& size) { m_Constraints.size = size; }
			inline const Vector2u& GetSize(void) const { return m_Constraints.size; }
			inline Type GetMeshType(void) const override { return Type::Box;}

		private:
			Box() = delete;
			Box(const Constraints& constraints);
			~Box() = default;

		private:
			Constraints m_Constraints;
		};
		class Circle : public BasicMesh 
		{
		public:
			struct Constraints : public BasicMesh::ColliderConstraints
			{
				Constraints(Alignment alignment, Vector2f position, float rotation, float radius) :
					BasicMesh::ColliderConstraints(alignment, position, rotation),
					radius(radius)
				{}
				~Constraints() = default;

				float radius = UNDEF_FLOAT;
			};

			static Box* Create(const Constraints& constraints);
			inline void SetRadius(float radius) { m_Constraints.radius = radius; }
			inline const float GetRadius(void) const { return m_Constraints.radius; }
			inline Type GetMeshType(void) const override { return Type::Circle; }

		private:
			Circle() = delete;
			Circle(const Constraints& constrains);
			~Circle() = default;

		private:
			Constraints m_Constraints;
		};

		static bool MeshTypesEquivalent(const Type& t1, const Type& t2);

	private:
		friend Collider;
	};
}

