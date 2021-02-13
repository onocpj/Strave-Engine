#include "DistanceRendering.h"

#include "RenderingConstraints.h"
#include "GameObject2D.h"
#include "Additive.h"
#include "WindowsWindow.h"
#include "Log.h"

#include <queue>
#include <math.h>

namespace Strave {

	namespace NRenderer {
		//=============================================================================
		//				class: RenderMap
		//=============================================================================

		DistanceRenderer::DistanceRenderer(const RenderingConstraints& constraints, const std::vector<GameObject2D*>& go2dCon, const Vector4f& normMapPoints)
			: m_NumberOfFields(RENDERING_GRID_DIM), m_NormalizedMapPoints(normMapPoints), 
			m_MapSize(UNDEF_VECTOR2F), m_Grid(UNDEF_STD_VECTOR), 
			m_RenderSection(UNDEF_SMARTPTR), m_GO2DContainerReference(UNDEF_PTR)
		{
			Init(constraints, go2dCon);
		}
		DistanceRenderer::~DistanceRenderer() {}

		void DistanceRenderer::Init(const RenderingConstraints& constraints, const std::vector<GameObject2D*>& go2dCon) {
			m_MapSize = GetMapSize();
			m_GO2DContainerReference = const_cast<std::vector<GameObject2D*>*>(&go2dCon);

			CreateSegments();

			m_RenderSection = std::make_shared<RenderSection>(constraints, m_Grid);
		}
		void DistanceRenderer::ShowSegmentContent(void) const {
			unsigned int t_TotalObjectNumber = UNDEF_UINT;
			Vector2f t_SegmentSize = UNDEF_VECTOR2U;

			std::cout << " =================================================== " << std::endl;
			std::cout << "|                                                   |" << std::endl;
			std::cout << "|                     RENDER MAP                    |" << std::endl;
			std::cout << "|                                                   |" << std::endl;
			std::cout << " =================================================== " << std::endl;

			for (std::vector<std::shared_ptr<Segment>>::iterator iter = const_cast<std::vector<std::shared_ptr<Segment>>*>(&m_Grid)->begin(),
				end = const_cast<std::vector<std::shared_ptr<Segment>>*>(&m_Grid)->end();
				iter != end;
				iter++) {

				t_SegmentSize = (*iter)->GetSize();

				std::cout << "=====================================================" << std::endl;
				std::cout << "                     SEGMENT (" << (*iter)->GetKey() << ")" << std::endl;
				std::cout << "=====================================================" << std::endl;
				std::cout << "Center         [x, y]: [" << (*iter)->GetCenterPoint().x << ", " << (*iter)->GetCenterPoint().y << "]" << std::endl;
				std::cout << "Start position [x, y]: [" << (*iter)->GetPosition().x << ", " << (*iter)->GetPosition().y << "]" << std::endl;
				std::cout << "End position   [x, y]: [" << (*iter)->GetPosition().x + t_SegmentSize.x << ", " << (*iter)->GetPosition().y + t_SegmentSize.y << "]" << std::endl;
				std::cout << "Size           [x, y]: [" << t_SegmentSize.x << ", " << t_SegmentSize.y << "]" << std::endl;
				std::cout << "Container Size       : " << (*iter)->GetContainerSize() << std::endl;
				std::cout << "-----------------------------------------------------" << std::endl;
				std::cout << "                   OBJECT CONTAINER                  " << std::endl;
				std::cout << std::endl;

				(*iter)->ShowContainerContent();

				std::cout << "=====================================================" << std::endl;
				std::cout << "////////////////////////////////////////////////////" << std::endl;

				t_TotalObjectNumber += (*iter)->GetContainerSize();

			}

			std::cout << "Total number of objects of all segments: " << t_TotalObjectNumber << std::endl;
		}
		Vector4f DistanceRenderer::GetHighestPoints(const std::vector<GameObject2D*>& go2dCon) {
			std::vector<GameObject2D*>& t_GO2DContainer = *(const_cast<std::vector<GameObject2D*>*>(&go2dCon));

			Vector4f t_HighestNormalizedPoints = UNDEF_VECTOR2V2F;
			Vector4f t_CurrentNormalizedPoints = UNDEF_VECTOR2V2F;

			// iterate throught the whole object container
			for (std::vector<GameObject2D*>::iterator iter = t_GO2DContainer.begin(), end = t_GO2DContainer.end(); iter != end; iter++) {

				// each iteration get
				t_CurrentNormalizedPoints.max = (*iter)->GetPosition();			
				t_CurrentNormalizedPoints.min = t_CurrentNormalizedPoints.max;  

				// find highest normalized points
				if (t_CurrentNormalizedPoints.max.x > t_HighestNormalizedPoints.max.x) {
					t_HighestNormalizedPoints.max.x = t_CurrentNormalizedPoints.max.x;
				}
				if (t_CurrentNormalizedPoints.max.y > t_HighestNormalizedPoints.max.y) {
					t_HighestNormalizedPoints.max.y = t_CurrentNormalizedPoints.max.y;
				}

				// find lowest normalized points
				if (t_CurrentNormalizedPoints.min.x < t_HighestNormalizedPoints.min.x) {
					t_HighestNormalizedPoints.min.x = t_CurrentNormalizedPoints.min.x;
				}
				if (t_CurrentNormalizedPoints.min.y < t_HighestNormalizedPoints.min.y) {
					t_HighestNormalizedPoints.min.y = t_CurrentNormalizedPoints.min.y;
				}

			}

			return t_HighestNormalizedPoints;
		}
		void DistanceRenderer::CreateSegments(void) {
			std::unique_ptr<std::vector<GameObject2D*>> t_UnassignedObjectContainer = std::make_unique<std::vector<GameObject2D*>>(*m_GO2DContainerReference);

			int		t_SegmentInitKey;
			Vector2f	t_SegmentSize;
			Vector2f	t_SegmentPosition;

			t_SegmentInitKey = INIT_RENDER_SECTION_KEY;
			t_SegmentSize = {
				m_MapSize.x / m_NumberOfFields.x,
				m_MapSize.y / m_NumberOfFields.y
			};
			t_SegmentPosition = {
				m_NormalizedMapPoints.min.x,
				m_NormalizedMapPoints.min.y
			};

			if (m_MapSize.x >= MIN_MAP_SIZE &&
				m_MapSize.y >= MIN_MAP_SIZE) {

				// create required number of sections
				for (unsigned int column = 0; column != m_NumberOfFields.y; column++) {
					for (unsigned int row = 0; row != m_NumberOfFields.x; row++) {

						// insert new section to container
						m_Grid.push_back(std::make_shared<Segment>(
							t_SegmentInitKey, t_SegmentSize, t_SegmentPosition, *GetObjectsInSegmentRange(t_SegmentPosition, t_SegmentSize, *t_UnassignedObjectContainer)
						));

						t_SegmentPosition.x += t_SegmentSize.x;	// update section x position
						t_SegmentInitKey++;			// update section key
					}

					t_SegmentPosition.x = m_NormalizedMapPoints.min.x;
					t_SegmentPosition.y += t_SegmentSize.y;		// update section y position
				}

			} else {
				// if map is smaller than min map dimensions, create only one section
				m_Grid.push_back(std::make_shared<Segment>(
					t_SegmentInitKey, t_SegmentSize, t_SegmentPosition, *GetObjectsInSegmentRange(t_SegmentPosition, t_SegmentSize, *t_UnassignedObjectContainer)
				));

			}
		}
		std::vector<GameObject2D*>* DistanceRenderer::GetObjectsInSegmentRange(const Vector2f& segPos, const Vector2f& segSize, std::vector<GameObject2D*>& unassignedObjects) {
			std::vector<GameObject2D*>* t_GO2DSectionContainer = new std::vector<GameObject2D*>();
			std::queue<unsigned int> t_AssignedObjectIndex;
			unsigned int t_Index = UNDEF_UINT;

			Vector2f t_ObjectPosition;
			Vector2f t_StartPosition;
			Vector2f t_EndPosition;

			t_ObjectPosition = UNDEF_VECTOR2F;
			t_StartPosition = segPos;
			t_EndPosition = {
				segPos.x + (float)segSize.x,
				segPos.y + (float)segSize.y
			};

			for (std::vector<GameObject2D*>::iterator iter = unassignedObjects.begin(), end = unassignedObjects.end(); iter != end; ++iter) {
				t_ObjectPosition = (*iter)->GetPosition();

				// check if objects position is higher than minimal position of section
				if (t_ObjectPosition.x >= t_StartPosition.x &&
					t_ObjectPosition.y >= t_StartPosition.y) {

					// chceck if objects position is less than maximal position of section
					if (t_ObjectPosition.x <= t_EndPosition.x &&
						t_ObjectPosition.y <= t_EndPosition.y) {

						t_GO2DSectionContainer->push_back(*iter);	// if so, push iteration of object to segment container
						t_AssignedObjectIndex.push(t_Index);		// get index of this iteration
					}
				}
				
				t_Index++;

			}

			// Erase all objects from the container that has been assigned
			Strave::Additive::ADDContainer::EraseElementsFromVector<GameObject2D*>(unassignedObjects, t_AssignedObjectIndex);

			return t_GO2DSectionContainer; // return created container
		}
		Vector2f DistanceRenderer::GetMapSize(void) const {
			Vector4f t_NormalizedMapSize = m_NormalizedMapPoints;
			Vector2f t_MapSize = UNDEF_VECTOR2F;

			if (t_NormalizedMapSize.max.x < 0.0f) {
				t_NormalizedMapSize.max.x *= (-1.0f);
			}
			if (t_NormalizedMapSize.max.y < 0.0f) {
				t_NormalizedMapSize.max.y *= (-1.0f);
			}

			t_MapSize.x = t_NormalizedMapSize.max.x - t_NormalizedMapSize.min.x;
			t_MapSize.y = t_NormalizedMapSize.max.y - t_NormalizedMapSize.min.y;

			return t_MapSize;
		}

		//=============================================================================
		//						    class: RenderMap::Segment
		//=============================================================================

		DistanceRenderer::Segment::Segment(SegmentKey_t key, const Vector2f& size, const Vector2f& pos, const std::vector<GameObject2D*>& go2dSegCon)
			: m_OccupancyState(UNDEF_BOOL), m_ContainerSize(UNDEF_UINT),
			m_Key(key), m_Size(size), m_Position(pos),
			m_CenterPoint(UNDEF_VECTOR2F), m_GO2DReferenceContainer(UNDEF_PTR)
		{
			Init(go2dSegCon);
		}
		DistanceRenderer::Segment::~Segment() {}

		void DistanceRenderer::Segment::Init(const std::vector<GameObject2D*>& go2dSegCon) {
			m_ContainerSize = (unsigned int)go2dSegCon.size();
			m_GO2DReferenceContainer = const_cast<std::vector<GameObject2D*>*>(&go2dSegCon);
			m_CenterPoint = InitCenterPoint(m_Position, m_Size);
			m_OccupancyState = m_ContainerSize > 0 ? true : false;
		}
		void DistanceRenderer::Segment::ShowContainerContent(void) const {
			for (auto iter = const_cast<std::vector<GameObject2D*>*>(m_GO2DReferenceContainer)->begin(),
				end = const_cast<std::vector<GameObject2D*>*>(m_GO2DReferenceContainer)->end(); iter != end; iter++) {

				// std::cout << "Object name: " << (*iter)->GetName() << std::endl;
				std::cout << "Object position [x, y]: [" << (*iter)->GetPosition().x << ", " << (*iter)->GetPosition().y << "]" << std::endl;
				std::cout << std::endl;
			}
		}
		Vector2f DistanceRenderer::Segment::InitCenterPoint(const Vector2f& pos, const Vector2f& size) const {
			return {
				((pos.x) + (pos.x + (float)size.x)) / 2,
				((pos.y) + (pos.y + (float)size.y)) / 2
			};
		}

		//=============================================================================
		//			class: RenderMap::RenderSection
		//=============================================================================

		DistanceRenderer::RenderSection::RenderSection(const RenderingConstraints& constraints, const std::vector<std::shared_ptr<Segment>>& grid)
			: m_PreloadRenderOffset(UNDEF_FLOAT), m_PreloadRenderDistance(UNDEF_FLOAT), m_RenderDistance(UNDEF_FLOAT),
			m_GridReference(UNDEF_SMARTPTR), m_RenderingArea(),
			m_RenderingGrid(UNDEF_SMARTPTR), m_RenderingBuffer(UNDEF_SMARTPTR)
		{
			Init(constraints, grid);
		}
		DistanceRenderer::RenderSection::~RenderSection() {}

		void DistanceRenderer::RenderSection::Init(const RenderingConstraints& constraints, const std::vector<std::shared_ptr<Segment>>& grid) {
			m_GridReference = const_cast<std::vector<std::shared_ptr<Segment>>*>(&grid);
			m_RenderingGrid = std::make_shared<DistanceRenderer::RenderSection::ComplexGrid_t>();
			m_RenderingBuffer = std::make_shared<RenderBuffer_t>();

			m_PreloadRenderDistance = VerifyRadius(constraints.GetPreloadedRadius());
			m_PreloadRenderOffset = GetPreloadRenderOffset();
			m_RenderDistance = VerifyRadius(constraints.GetRenderRadius());

			m_RenderingArea.RenderArea.SetPosition(constraints.GetPoint());
			m_RenderingArea.RenderArea.SetRadius(m_RenderDistance);
			m_RenderingArea.PreloadArea.SetPosition(constraints.GetPoint());
			m_RenderingArea.PreloadArea.SetRadius(m_PreloadRenderDistance);

			PreloadRenderingGrid();
		}
		void DistanceRenderer::RenderSection::Render(WindowsWindow& window) {
			for (objectlist_iter object = m_RenderingBuffer->Loaded->begin(), end = m_RenderingBuffer->Loaded->end(); object != end; ++object) 
			{
				if (Shapes::fPoint::IsInRange<float>(
					Shapes::fPoint::DistanceBetweenPoints<float>(m_RenderingArea.RenderArea.GetPostion(), (*object)->GetPosition()),
					m_RenderDistance
				))
				{
					Animation::Update(**object);
					window.Draw(*((*object)->GetModel<sf::Sprite>()));
				}
			}
		}
		state_t DistanceRenderer::RenderSection::Update(void) {
			if ((*m_RenderingGrid->InRange).size() < (*m_RenderingGrid->NotInRange).size()) {
				return OptimizedUpdate();

			} else {
				return RawUpdate();
			}
		}
		state_t DistanceRenderer::RenderSection::OptimizedUpdate(void) {
			// Instad of running two independant loops for complex grid container "m_RenderingGrid" and updating
			// its elements "InRange" and "NotInRange", algorithm is running one loop, that updates both elements each cycle.
			// This should help to reduce number of loops that needs to be done to preload elements
			//
			// Side effect of this is creating and copying a bunch of temporary data

			Vector2f t_InnerCirclePosition = m_RenderingArea.RenderArea.GetPostion();

			if (Shapes::Circle::InternalCircleBoundingTest(
				t_InnerCirclePosition, m_RenderingArea.PreloadArea.GetPostion(),
				m_RenderDistance, m_PreloadRenderDistance))
			{
				unsigned t_IetrationA = UNDEF_UINT;
				unsigned t_IetrationB = UNDEF_UINT;
				unsigned t_ContainerSize = UNDEF_UINT;

				size_t t_InRangeContainerSize = m_RenderingGrid->InRange->size();
				size_t t_NotInRangeContainerSize = m_RenderingGrid->NotInRange->size();

				rendergrid_t t_PreloadedInRange = *m_RenderingGrid->InRange;
				rendergrid_t t_PreloadedNotInRange = *m_RenderingGrid->NotInRange;

				m_RenderingArea.PreloadArea.SetPosition(m_RenderingArea.RenderArea.GetPostion());
				m_RenderingBuffer->Preloaded->clear();

				for (unsigned int index = 0; index < t_NotInRangeContainerSize; index++) {
					// check if segments those were assigned to "preload rendering range conntainer", are still in the preload rendering range
					if (index < t_InRangeContainerSize) {
						// if assigned segment is no longer in preload rendering range
						if (!Shapes::fPoint::IsInRange<float>(
							Shapes::fPoint::DistanceBetweenPoints<float>(m_RenderingArea.PreloadArea.GetPostion(), m_RenderingGrid->InRange->at(index)->GetCenterPoint()),
							m_PreloadRenderDistance + m_PreloadRenderOffset
							))
						{
							t_PreloadedNotInRange.push_back(m_RenderingGrid->InRange->at(index));
							t_PreloadedInRange.erase(t_PreloadedInRange.begin() + (index - t_IetrationA));

							t_IetrationA++;

						}
						else {
							m_RenderingBuffer->Preloaded->insert(
								m_RenderingBuffer->Preloaded->end(),
								GetObjectContainer(*m_RenderingGrid->InRange->at(index)).begin(),
								GetObjectContainer(*m_RenderingGrid->InRange->at(index)).end()
							);
						}
					}

					// chceck if segments, those were not assigned yet to "preload rendering range conntainer", are in the preload rendering range
					if (Shapes::fPoint::IsInRange<float>(
						Shapes::fPoint::DistanceBetweenPoints<float>(m_RenderingArea.PreloadArea.GetPostion(), m_RenderingGrid->NotInRange->at(index)->GetCenterPoint()),
						m_PreloadRenderDistance + m_PreloadRenderOffset
						))
					{
						t_PreloadedInRange.push_back(m_RenderingGrid->NotInRange->at(index));
						t_PreloadedNotInRange.erase(t_PreloadedNotInRange.begin() + (index - t_IetrationB));

						m_RenderingBuffer->Preloaded->insert(
							m_RenderingBuffer->Preloaded->end(),
							GetObjectContainer(*m_RenderingGrid->NotInRange->at(index)).begin(),
							GetObjectContainer(*m_RenderingGrid->NotInRange->at(index)).end()
						);																			// insert all obbjects of this segment to preload rendering buffer

						t_IetrationB++;
					}
				}

				*m_RenderingGrid->InRange = t_PreloadedInRange;
				*m_RenderingGrid->NotInRange = t_PreloadedNotInRange;

				m_RenderingBuffer->Loaded = m_RenderingBuffer->Preloaded; // In game engine, send this line of code to mutex

				/*thread::ThreadPool::SendToMutex([this](){
					m_RenderingBuffer->Loaded = m_RenderingBuffer->Preloaded;
				});*/

				SV_CORE_TRACE("Rendering: Optimalized");
				return true;
			}

			return false;
		}
		state_t DistanceRenderer::RenderSection::RawUpdate(void) {
			Vector2f t_InnerCirclePosition = m_RenderingArea.RenderArea.GetPostion();

			if (Shapes::Circle::InternalCircleBoundingTest(
				t_InnerCirclePosition, m_RenderingArea.PreloadArea.GetPostion(),
				m_RenderDistance, m_PreloadRenderDistance))
			{
				unsigned t_IetrationA = UNDEF_UINT;
				unsigned t_IetrationB = UNDEF_UINT;

				size_t t_InRangeContainerSize = m_RenderingGrid->InRange->size();
				size_t t_NotInRangeContainerSize = m_RenderingGrid->NotInRange->size();

				rendergrid_t t_PreloadedInRange = *m_RenderingGrid->InRange;
				rendergrid_t t_PreloadedNotInRange = *m_RenderingGrid->NotInRange;

				m_RenderingArea.PreloadArea.SetPosition(m_RenderingArea.RenderArea.GetPostion());
				m_RenderingBuffer->Preloaded->clear();

				for (unsigned int index = 0; index < t_InRangeContainerSize; index++) {
					// check if segments those were assigned to "preload rendering range conntainer", are still in the preload rendering range
					if (index < t_InRangeContainerSize) {
						// if assigned segment is no longer in preload rendering range
						if (!Shapes::fPoint::IsInRange<float>(
							Shapes::fPoint::DistanceBetweenPoints<float>(m_RenderingArea.PreloadArea.GetPostion(), m_RenderingGrid->InRange->at(index)->GetCenterPoint()),
							m_PreloadRenderDistance + m_PreloadRenderOffset
							))
						{
							t_PreloadedNotInRange.push_back(m_RenderingGrid->InRange->at(index));
							t_PreloadedInRange.erase(t_PreloadedInRange.begin() + (index - t_IetrationA));

							t_IetrationA++;

						} else {
							m_RenderingBuffer->Preloaded->insert(
								m_RenderingBuffer->Preloaded->end(),
								GetObjectContainer(*m_RenderingGrid->InRange->at(index)).begin(),
								GetObjectContainer(*m_RenderingGrid->InRange->at(index)).end()
							);
						}
					}
				}
				for (unsigned int index = 0; index < t_NotInRangeContainerSize; index++) {
					// chceck if segments, those were not assigned yet to "preload rendering range conntainer", are in the preload rendering range
					if (Shapes::fPoint::IsInRange<float>(
						Shapes::fPoint::DistanceBetweenPoints<float>(m_RenderingArea.PreloadArea.GetPostion(), m_RenderingGrid->NotInRange->at(index)->GetCenterPoint()),
						m_PreloadRenderDistance + m_PreloadRenderOffset
						))
					{
						t_PreloadedInRange.push_back(m_RenderingGrid->NotInRange->at(index));
						t_PreloadedNotInRange.erase(t_PreloadedNotInRange.begin() + (index - t_IetrationB));

						m_RenderingBuffer->Preloaded->insert(
							m_RenderingBuffer->Preloaded->end(),
							GetObjectContainer(*m_RenderingGrid->NotInRange->at(index)).begin(),
							GetObjectContainer(*m_RenderingGrid->NotInRange->at(index)).end()
						);																			// insert all obbjects of this segment to preload rendering buffer

						t_IetrationB++;																// insert all obbjects of this segment to preload rendering buffer
					}
				}

				*m_RenderingGrid->InRange = t_PreloadedInRange;
				*m_RenderingGrid->NotInRange = t_PreloadedNotInRange;

				m_RenderingBuffer->Loaded = m_RenderingBuffer->Preloaded; // In game engine, send this line of code to mutex

				/*thread::ThreadPool::SendToMutex([this]() {
					m_RenderingBuffer->Loaded = m_RenderingBuffer->Preloaded;
				});*/

				SV_CORE_TRACE("Rendering: Raw");
				return true;
			}

			return false;
		}
		void DistanceRenderer::RenderSection::PreloadRenderingGrid(void) {
			if (m_RenderingGrid != UNDEF_SMARTPTR) {

				for (grid_iter segment = m_GridReference->begin(), end = m_GridReference->end(); segment != end; ++segment) {

					// check if segment is occupied
					if ((*segment)->IsOccupied()) {

						// check if segment is in rendering range
						if (Shapes::fPoint::IsInRange<float>(
							Shapes::fPoint::DistanceBetweenPoints<float>(m_RenderingArea.PreloadArea.GetPostion(), (*segment)->GetCenterPoint()),
							m_PreloadRenderDistance + m_PreloadRenderOffset
						))
						{
							// if so, push segment to rendering area container, so its known, that this segment is being rendered
							m_RenderingGrid->InRange->push_back(*segment);
							// insert all obbjects of this segment to rendering conatiner
							m_RenderingBuffer->Preloaded->insert(
								m_RenderingBuffer->Preloaded->end(),
								GetObjectContainer(**segment).begin(),
								GetObjectContainer(**segment).end()
							);

						} else { // if segment is not in renderign range
							// insert this segment to  not in range container
							m_RenderingGrid->NotInRange->push_back(*segment);
						}

					}

				}

				// insert created(preloaded) rendering buffer to final rendering buffer
				m_RenderingBuffer->Loaded = m_RenderingBuffer->Preloaded;

			} else {
				SV_CORE_WARN("function DistanceRendering::RenderSection::PreloadRenderingGrid: Rendering area was undefined");
			}
		}
		float DistanceRenderer::RenderSection::VerifyRadius(float radius) const {
			return radius >= MIN_RADIUS ? radius : MIN_RADIUS;
		}
		float DistanceRenderer::RenderSection::GetPreloadRenderOffset(void) const {
			float t_Diagonal = UNDEF_FLOAT;

			t_Diagonal = (float)sqrt(
				pow(m_GridReference->at(0)->GetSize().x, 2) +
				pow(m_GridReference->at(0)->GetSize().y, 2)
			);

			return t_Diagonal / 2;
		}

	}

}
