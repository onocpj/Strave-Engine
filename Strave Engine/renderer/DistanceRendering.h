#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../core/datatypes/Vector2.h"
#include "../core/datatypes/Vector4.h"
#include "../core/datatypes/shapes/Shapes.h"


#define INIT_RENDER_SECTION_KEY				0
#define	MIN_MAP_SIZE						100.0f
#define MIN_RADIUS							1.0f
#define RENDERING_GRID_DIM					{ 40, 40 }
#define MAX_RENDER_DISTANCE					5000.0f
#define PRELOAD_RENDER_DISTANCE_OFFSET		20.0f	


typedef bool state_t;


namespace Strave 
{
	class WindowsWindow;
	class GameObject2D;

	namespace nRenderer 
	{
		class RenderingConstraints;
		class DistanceRenderer {

		private:
			class Segment;
			class RenderSection;

			typedef int32_t SegmentKey_t;

		public:
			DistanceRenderer(const RenderingConstraints& constraints, const std::vector<GameObject2D*>& go2dCon, const Vector4f& normMapPoints);
			~DistanceRenderer();

			void ShowSegmentContent(void) const;

			static Vector4f GetHighestPoints(const std::vector<GameObject2D*>& go2dCon);
			inline std::shared_ptr<RenderSection> GetRenderSection(void) { return m_RenderSection; }

		private:
			class RenderSection {
			private:
				typedef std::vector<GameObject2D*>					objectlist_t;
				typedef std::vector<std::shared_ptr<Segment>>		rendergrid_t;
				typedef std::vector<std::shared_ptr<Segment>>*		rendergridptr_t;

				typedef std::vector<std::shared_ptr<Segment>>::iterator		grid_iter;
				typedef std::vector<GameObject2D*>::iterator				objectlist_iter;

				typedef struct ComplexGrid {

					inline ComplexGrid()
						: InRange(std::make_shared<rendergrid_t>()),
						NotInRange(std::make_shared<rendergrid_t>())
					{}
					inline ~ComplexGrid() {}

					std::shared_ptr<rendergrid_t> InRange;
					std::shared_ptr<rendergrid_t> NotInRange;

				} ComplexGrid_t;
				typedef struct RenderBuffer {

					inline RenderBuffer()
						: Preloaded(std::make_shared<objectlist_t>()),
						Loaded(Preloaded)
					{}
					inline ~RenderBuffer() {}

					std::shared_ptr<objectlist_t> Preloaded;
					std::shared_ptr<objectlist_t> Loaded;

				} RenderBuffer_t;
				typedef struct RenderingArea {

					Strave::Shapes::Circle RenderArea;
					Strave::Shapes::Circle PreloadArea;

				} RenderingArea_t;

			public:
				RenderSection(const RenderingConstraints& constraints, const std::vector<std::shared_ptr<Segment>>& grid);
				~RenderSection();

				inline void SetRenderDistance(float radius) { radius >= MIN_RADIUS ? m_PreloadRenderDistance = radius : 0; }

				inline std::shared_ptr<objectlist_t>& GetObjectsToRender() const { return m_RenderingBuffer->Loaded; }
				inline const Vector2f& GetPointPosition(void) const { return m_RenderingArea.RenderArea.GetPostion(); }
				inline const Vector2f& GetCenterPoint(void) const { return m_RenderingArea.PreloadArea.GetPostion(); }
				inline const float GetRenderDistance(void) const { return m_RenderDistance; };
				inline const std::vector<GameObject2D*>& GetObjectContainer(const Segment& seg) const { return *seg.m_GO2DReferenceContainer; }
	
				void Render(WindowsWindow& window);	// critical function (runtime function)

				#define RENDERING_UPDATE(RENDERER_REF) RENDERER_REF->Update();		
				state_t Update(void);

				state_t OptimizedUpdate(void);		// critical function (runtime function)
				state_t RawUpdate(void);			// critical function (runtime function)
				inline void UpdatePointPosition(const Vector2f& pointPosition) { m_RenderingArea.RenderArea.SetPosition(pointPosition); }		

			private:
				void Init(const RenderingConstraints& constraints, const std::vector<std::shared_ptr<Segment>>& grid);
				void PreloadRenderingGrid(void);
				float VerifyRadius(float radius) const;
				float GetPreloadRenderOffset(void) const;
				inline void LoadPreloadedBuffer(void) { m_RenderingBuffer->Loaded = m_RenderingBuffer->Preloaded; }


				float							m_PreloadRenderOffset;
				float							m_PreloadRenderDistance;
				float							m_RenderDistance;
				rendergridptr_t					m_GridReference;
				RenderingArea_t					m_RenderingArea;
				std::shared_ptr<ComplexGrid_t>	m_RenderingGrid;
				std::shared_ptr<RenderBuffer_t>	m_RenderingBuffer;
			};
			class Segment {
			public:
				Segment(SegmentKey_t key, const Vector2f& size, const Vector2f& pos, const std::vector<GameObject2D*>& go2dSegCon);
				~Segment();

				void ShowContainerContent(void) const;
				inline unsigned int GetContainerSize() { return m_ContainerSize; }
				inline int GetKey(void) const { return m_Key; }
				inline const Vector2f& GetCenterPoint(void) const { return m_CenterPoint; }
				inline const Vector2f& GetSize(void) const { return m_Size; }
				inline const Vector2f& GetPosition(void) const { return m_Position; }
				inline bool IsOccupied(void) const { return m_OccupancyState; };

			private:
				void Init(const std::vector<GameObject2D*>& go2dSegCon);
				Vector2f InitCenterPoint(const Vector2f& pos, const Vector2f& size) const;

				friend const std::vector<GameObject2D*>& RenderSection::GetObjectContainer(const Segment& seg) const;


				bool						m_OccupancyState;
				unsigned int				m_ContainerSize;
				SegmentKey_t				m_Key;
				Vector2f					m_Size;
				Vector2f					m_Position;
				Vector2f					m_CenterPoint;
				std::vector<GameObject2D*>* m_GO2DReferenceContainer;

			};

			void Init(const RenderingConstraints& constraints, const std::vector<GameObject2D*>& go2dCon);
			std::vector<GameObject2D*>* GetObjectsInSegmentRange(const Vector2f& segPos, const Vector2f& segSize, std::vector<GameObject2D*>& unassignedObjects);
			void CreateSegments(void);
			Vector2f GetMapSize(void) const;


			Vector2u								m_NumberOfFields;
			Vector4f								m_NormalizedMapPoints;
			Vector2f								m_MapSize;
			std::vector<std::shared_ptr<Segment>>	m_Grid;
			std::shared_ptr<RenderSection>			m_RenderSection;
			std::vector<GameObject2D*>*				m_GO2DContainerReference;

		};

	}

}


