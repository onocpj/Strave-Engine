#include "UserInterface.h"

#include "Application.h"
#include "WindowsWindow.h"
#include "GameObject2D.h"
#include "UDT.h"
#include "Convert.h"
#include "Vector2.h"
#include "Log.h"

#include <iostream>


namespace Strave {

	extern Strave::Application*	StraveApplication;


	namespace GUI {

		//========================================================================================
		//					class: Interface
		//========================================================================================

		UserInterface::UserInterface()
			: GameObject2D(),
			m_Mouse(UNDEF_PTR), m_ShowState(INIT_DEF_VIEW_STATE), m_WindowReference(UNDEF_PTR)
		{
			Init(*Strave::StraveApplication->GetWindow());
		}
		UserInterface::UserInterface(OBJECT_TYPE objType, std::weak_ptr<sf::Texture> texture, std::weak_ptr<sf::Sprite> sprite, std::weak_ptr<sf::RectangleShape> rectSprite, const Strave::Vector2f& position, const Strave::Vector2f& spriteScale)
			: GameObject2D(objType, texture, sprite, rectSprite, position, spriteScale),
			m_Mouse(UNDEF_PTR), m_ShowState(INIT_DEF_VIEW_STATE), m_WindowReference(UNDEF_PTR)
		{
			Init(*Strave::StraveApplication->GetWindow());
		}
		UserInterface::~UserInterface() {}

		//void Hide(void) {

		//	// if interface is drawn in the window
		//	if (this->m_ShowState == true) {
		//		Strave::thread::ThreadPool::s_Pool->Enqueue([this]() {																	// enqueue new task 
		//			Strave::StraveApplication->GetRenderer()->EraseObjectFromScene(*this, Strave::renderer::RENDERER_OBJECT_TYPE::INTERFACE);	// remove interface from scene 
		//		});

		//		this->m_ShowState = false;
		//	}
		//}

		void UserInterface::Init(WindowsWindow& window) {

			m_Mouse = std::make_shared<Mouse>(window);

		}
		void UserInterface::Draw(void) const {

			Strave::StraveApplication->GetWindow()->Draw<sf::RectangleShape>(*this->GetModel<sf::RectangleShape>());

		}
		bool UserInterface::MouseClick(Mouse::Button button) const {

			// if show state of instance is set to true
			if (this->m_ShowState == true) {
				return m_Mouse->Click(button, *this); // return mouse state
			}

			return false; // click state return false

		}
		bool UserInterface::MouseDoubleClick(Mouse::Button button) const {

			if (this->m_ShowState == true) {
				return m_Mouse->DoubleClick(button, *this);
			}
			
			return false;

		}
		bool UserInterface::MouseDoubleClick(Mouse::Button button, float clickDelay) const {

			if (this->m_ShowState == true) {
				return m_Mouse->DoubleClick(button, clickDelay, *this);
			}

			return false;

		}
		bool UserInterface::MouseReleased(Mouse::Button button) const {

			if (this->m_ShowState == true) {
				return m_Mouse->Released(button, *this);
			}

			return false;

		}
		bool UserInterface::MouseHold(Mouse::Button button) const {

			if (this->m_ShowState == true) {
				return m_Mouse->Hold(button);
			}

			return false;

		}
		bool UserInterface::MouseHover(void) {

			// if showstate of this instance is true
			if (this->m_ShowState == true) {
				return m_Mouse->Hover(*this);
			}

			return false; // if mouse cursor is not hovering interface instance return false

		}
		bool UserInterface::MouseDrag(void) {

			if (this->m_ShowState == true) {
				return m_Mouse->Drag(*this);
			}

			return false;

		}

		//========================================================================================
		//					   class: Cell
		//========================================================================================

		Cell::Cell(std::weak_ptr<sf::Texture> texture, std::weak_ptr<sf::Sprite> sprite, std::weak_ptr<sf::RectangleShape> rectSprite, const Strave::Vector2f& position, const Strave::Vector2f& spriteSize)
			: UserInterface(OBJECT_TYPE::UI_CELL, texture, sprite, rectSprite, position, spriteSize),
			m_CellElement(UNDEF_PTR)
		{
			m_CellElement = Element::Create(*this);
		}
		Cell::Cell()
			: UserInterface(),
			m_CellElement(UNDEF_PTR)
		{}

		Cell* Cell::Create(sf::Texture& texture, const Strave::Vector2f& position, const Strave::Vector2f& spriteSize) {

			// creating cell
			std::shared_ptr<sf::Sprite> t_Sprite2D = std::make_shared<sf::Sprite>();
			std::shared_ptr<sf::RectangleShape> t_RectSprite2D = std::make_shared<sf::RectangleShape>();

			t_Sprite2D->setTexture(texture);
			t_Sprite2D->setScale(Convert::ToVector2(spriteSize));
			t_Sprite2D->setPosition(Convert::ToVector2(position));

			t_RectSprite2D->setTexture(&texture);
			t_RectSprite2D->setSize(Convert::ToVector2(spriteSize));
			t_RectSprite2D->setPosition(Convert::ToVector2(position));

			// creating cells miniature
			

			return new Cell(std::make_shared<sf::Texture>(texture), t_Sprite2D, t_RectSprite2D, position, spriteSize); // return cell

		}
		void Cell::Draw() const {

			StraveApplication->GetWindow()->Draw<sf::RectangleShape>(*this->GetModel<sf::RectangleShape>()); // draw cell

			if (!this->IsEmpty()) {
				StraveApplication->GetWindow()->Draw<sf::RectangleShape>(*m_CellElement->GetModel<sf::RectangleShape>()); // draw element
			}

		}
		void Cell::AddElement(const GameObject2D& obj) const {

			m_CellElement->Add(obj);

		}
		void Cell::RemoveElement(void) const {

			m_CellElement->Remove();

		}

		//========================================================================================
		//				    class: Cell::Element
		//========================================================================================

		Cell::Element::Element(std::weak_ptr<sf::Texture> texture, std::weak_ptr<sf::Sprite> sprite, std::weak_ptr<sf::RectangleShape> rectSprite, const Strave::Vector2f& position, const Strave::Vector2f& spriteSize)
			: GameObject2D(OBJECT_TYPE::UI_ELEMENT, texture, sprite, rectSprite, position, spriteSize), 
			m_GO2DReference(UNDEF_PTR)
		{}
		Cell::Element::Element()
			: GameObject2D(), 
			m_GO2DReference(UNDEF_PTR)
		{}
		Cell::Element::~Element() {}

		Cell::Element* Cell::Element::Create(const Cell& cell) {

			std::shared_ptr<sf::Texture> t_Texture2D = std::make_shared<sf::Texture>();
			std::shared_ptr<sf::Sprite> t_Sprite2D = std::make_shared<sf::Sprite>();
			std::shared_ptr<sf::RectangleShape> t_RectSprite2D = std::make_shared<sf::RectangleShape>();

			Vector2f t_ElementSize;
			Vector2f t_ElementPosition;

			t_ElementSize = cell.GetRectSpriteSize() * (ELEMENT_MARGINS_PERCENTAGE / 100.0f);
			t_ElementPosition = {
				cell.GetPosition().x + cell.GetRectSpriteSize().x - t_ElementSize.x,
				cell.GetPosition().y + cell.GetRectSpriteSize().y - t_ElementSize.y,
			};

			std::cout << cell.GetRectSpriteSize().x << " " << cell.GetRectSpriteSize().y << std::endl;
			std::cout << t_ElementSize.x << " " << t_ElementSize.y << std::endl;

			t_Sprite2D->setTexture(*t_Texture2D);
			t_Sprite2D->setScale(Convert::ToVector2(t_ElementSize));
			t_Sprite2D->setPosition(Convert::ToVector2(t_ElementPosition));

			t_RectSprite2D->setTexture(&(*t_Texture2D));
			t_RectSprite2D->setSize(Convert::ToVector2(t_ElementSize));
			t_RectSprite2D->setPosition(Convert::ToVector2(t_ElementPosition));

			return new Element(t_Texture2D, t_Sprite2D, t_RectSprite2D, t_ElementPosition, t_ElementSize);

		}
		void Cell::Element::Add(const GameObject2D& obj) {

			m_GO2DReference = &obj;
			this->UpdateTexture(obj.GetTexture());

		}
		void Cell::Element::Remove(void) {

			m_GO2DReference = UNDEF_PTR;
			this->UpdateTexture(GameObject2D::s_EmptyTexture);

		}

		//========================================================================================
		//					class: Table
		//========================================================================================

		Grid::Grid(const Strave::Vector2u& dimensions, sf::Texture& cellTexture, std::weak_ptr<sf::Texture> tableTexture, std::weak_ptr<sf::Sprite> sprite, std::weak_ptr<sf::RectangleShape> rectSprite, const Strave::Vector2f& position, const Strave::Vector2f& spriteSize)
			: UserInterface(OBJECT_TYPE::UI_TABLE, tableTexture, sprite, rectSprite, position, spriteSize),
			m_RowsNumber(dimensions.x), m_ColumnsNumber(dimensions.y), m_Cell(UNDEF_STD_VECTOR), m_MarginsSize(NO_MARGINS)
		{
			AlignCells(dimensions.x, dimensions.y, cellTexture, position, spriteSize);
		}
		Grid::Grid() 
			: UserInterface(),
			m_RowsNumber(UNDEF_UINT), m_ColumnsNumber(UNDEF_UINT), m_Cell(UNDEF_STD_VECTOR), m_MarginsSize(UNDEF_VECTOR2F)
		{}

		Grid* Grid::Create(const Strave::Vector2u& dimensions, sf::Texture& gridTexture, sf::Texture& cellTexture, const Strave::Vector2f& position, const Strave::Vector2f& spriteSize) {

			std::shared_ptr<sf::Sprite> t_Sprite2D = std::make_shared<sf::Sprite>();
			std::shared_ptr<sf::RectangleShape> t_RectSprite2D = std::make_shared<sf::RectangleShape>();

			t_Sprite2D->setTexture(gridTexture);
			t_Sprite2D->setScale(Convert::ToVector2(spriteSize));
			t_Sprite2D->setPosition(Convert::ToVector2(position));

			t_RectSprite2D->setTexture(&gridTexture);
			t_RectSprite2D->setSize(Convert::ToVector2(spriteSize));
			t_RectSprite2D->setPosition(Convert::ToVector2(position));

			return new Grid(dimensions, cellTexture, std::make_shared<sf::Texture>(gridTexture), t_Sprite2D, t_RectSprite2D, position, spriteSize);

		}
		const GameObject2D* Grid::GetObject(const Cell& cell) {

			if (!cell.IsEmpty()) {
				return cell.GetElement()->GetObject();
			}

			return UNDEF_PTR;

		}
		const GameObject2D* Grid::GetObject(unsigned int index) {

			if (!m_Cell.at(index)->IsEmpty()) {
				return m_Cell.at(index)->GetElement()->GetObject();
			}

			return UNDEF_PTR;

		}
		void Grid::SetMargins(const Strave::Vector2f& margins) {

			unsigned int t_RowCounter;
			unsigned int t_ColumnCounter;

			Strave::Vector2f t_SnipSize;
			Strave::Vector2f t_CellSize;
			Strave::Vector2f t_TablePosition;
			Strave::Vector2f t_CellPosition;

			m_MarginsSize = margins;
			t_RowCounter = UINT_BEGINING;
			t_ColumnCounter = UINT_BEGINING;

			// Value that represents table edge size for both axis
			// Those margins are times by two
			// This will ensure that there will be enough space for the side edges and upper and lower edges
			t_SnipSize = {
				(2 * margins.x) / m_ColumnsNumber,
				(2 * margins.y) / m_RowsNumber
			};
			// Value represents size of the each cell
			// Value is set to the table size devided by number of cells in rows and columns, minus size of the added edges
			// This give us average size of cell to be able to fit this cells to table with edges
			t_CellSize = {
				m_Cell.at(VECTOR_BEGINING)->GetRectSpriteSize().x - t_SnipSize.x,
				m_Cell.at(VECTOR_BEGINING)->GetRectSpriteSize().y - t_SnipSize.y
			};
			// Value that represents position of the table
			// Value is base position of table plus edge size
			// This will ensure that cell will be put to table starting position, with left space for upper and side edge
			t_TablePosition = {
				this->GetPosition().x + margins.x,
				this->GetPosition().y + margins.y
			};
			// Value represents position of the first cell 
			t_CellPosition = {
				t_TablePosition.x,
				t_TablePosition.y
			};

			// Cycle whole container of cells
			for (std::vector<std::shared_ptr<Cell>>::iterator iter = m_Cell.begin(); iter != m_Cell.end(); iter++) {
				// If column counter is less than number of final columns
				if (t_ColumnCounter < m_ColumnsNumber) {

					(*iter)->SetPosition(t_CellPosition);	// Set position of iterator cell to new position
					(*iter)->SetSpriteSize(t_CellSize);	// Set sprite size of this cell, to be able to fit cells in one row, with enough space for side edges

					// Set cell position to last cell position, plus size of one cell
					// This will ensure that next cell in row will be right next to the last one
					t_CellPosition.x += t_CellSize.x;

					t_ColumnCounter++; // Count nuumber of columns in one row

				}
				else { // If you filled one row

					t_ColumnCounter = UINT_BEGINING; 	// Reset value, that represents cells in one row
					t_CellPosition.x = t_TablePosition.x;	// Reset cell x axis position. So next row of cells can start again from edge of the table 
					t_CellPosition.y += t_CellSize.y;	// Set y axis cell position to last cell position plus cell y size. This will ensure that next row of cells will be right under the last one

					t_RowCounter++; // Count number of rows
					iter--; // When we jump to initializing next row of cells, no cell is iniintialized, but iterator is still counting. We need to uncount this iterator when we cycle each row

				}

			}

		}
		void Grid::SetGridPosition(const Strave::Vector2f& position) {

			unsigned int t_RowCounter;
			unsigned int t_ColumnCounter;

			Strave::Vector2f t_TablePosition;
			Strave::Vector2f t_CellPosition;
			Strave::Vector2f t_CellSize;

			t_RowCounter = UINT_BEGINING;
			t_ColumnCounter = UINT_BEGINING;

			// Value that represents position of the table
			// Value is base position of table plus edge size
			// This will ensure that cell will be put to table starting position, with left space for upper and side edge
			t_TablePosition = {
				position.x + m_MarginsSize.x,
				position.y + m_MarginsSize.y
			};
			// Value represents position of the first cell 
			t_CellPosition = {
				t_TablePosition.x,
				t_TablePosition.y
			};
			// Value represents size of the each cell
			// Value is set to the cell size on x and y axis
			// Because each cell have same size, we are using size of the first cell from container as reference
			t_CellSize = {
				m_Cell.at(VECTOR_BEGINING)->GetRectSpriteSize().x,
				m_Cell.at(VECTOR_BEGINING)->GetRectSpriteSize().y
			};

			this->SetPosition(position); // Sets position of table backround

			// Cycle whole cell container
			for (std::vector<std::shared_ptr<Cell>>::iterator cell = m_Cell.begin(); cell != m_Cell.end(); cell++) {
				// If column counter is less than number of final columns
				if (t_ColumnCounter < m_ColumnsNumber) {
					(*cell)->SetPosition(Strave::Vector2f(t_CellPosition.x, t_CellPosition.y)); // Set position of cell to the new position

					auto t_Element = *(*cell)->GetElement();

					t_Element.SetPosition(Strave::Vector2f(
						t_CellPosition.x + t_CellSize.x - t_Element.GetRectSpriteSize().x,
						t_CellPosition.y + t_CellSize.y - t_Element.GetRectSpriteSize().y
					));																			// Set position of elements

					// Set cell position to last cell position, plus size of one cell
					// This will ensure that next cell in row will be right next to the last one
					t_CellPosition.x += t_CellSize.x;

					t_ColumnCounter++; // Count nuumber of columns in one row

				}
				else { // If you filled one row

					t_ColumnCounter = UINT_BEGINING;	// Reset value, that represents cells in one row
					t_CellPosition.x = t_TablePosition.x;	// Reset cell x axis position. So next row of cells can start again from edge of the table 
					t_CellPosition.y += t_CellSize.y;	// Set y axis cell position to last cell position plus cell y size. This will ensure that next row of cells will be right under the last one

					t_RowCounter++; // Count number of rows
					cell--; // When we jump to initializing next row of cells no cell is iniintialized, but iterator is still counting. We need to uncount this iterator when we cycle each row

				}
			}

		}
		void Grid::AlignCells(unsigned int rowsNumber, unsigned int columnsNumber, sf::Texture& cellTexture, const Strave::Vector2f& tablePosition, const Strave::Vector2f& tableSize) {

			unsigned int t_RowCounter;
			unsigned int t_ColumnCounter;

			Strave::Vector2f t_CellPosition;
			Strave::Vector2f t_CellSize;

			t_RowCounter = UINT_BEGINING;
			t_ColumnCounter = UINT_BEGINING;

			// Value represents position of the first cell 
			t_CellPosition = {
				tablePosition.x,
				tablePosition.y
			};
			// Value represents size of the each cell
			// Value is set to the table size devided by number of cells in rows and columns
			// This give us average size of cell to be able to fit this cells to table
			t_CellSize = {
				(tableSize.x / columnsNumber),
				(tableSize.y / rowsNumber)
			};

			// Cycle to initialize each cell
			for (unsigned int i = 0; i != (rowsNumber * columnsNumber); i++) {
				// If column counter is less than number of final columns
				if (t_ColumnCounter < columnsNumber) {

					// Create new cell and push it to the container
					m_Cell.push_back(std::make_shared<Cell>(*Cell::Create(cellTexture, Strave::Vector2f(t_CellPosition.x, t_CellPosition.y), Strave::Vector2f(t_CellSize.x, t_CellSize.y))));

					// Set cell position to last cell position, plus size of one cell
					// This will ensure that next cell in row will be right next to the last one
					t_CellPosition.x += t_CellSize.x;

					t_ColumnCounter++; // Count nuumber of columns in one row

				}
				else { // If you filled one row

					t_ColumnCounter = UINT_BEGINING;	// Reset value, that represents cells in one row
					t_CellPosition.x = tablePosition.x;	// Reset cell x axis position. So next row of cells can start again from edge of the table 
					t_CellPosition.y += t_CellSize.y;	// Set y axis cell position to last cell position plus cell y size. This will ensure that next row of cells will be right under the last one


					t_RowCounter++; // Count number of rows
					i--; // When we jump to initializing next row of cells, no cell is iniintialized, but iterator is still counting. We need to uncount this iterator when we cycle each row

				}

			}

		}
		void Grid::Insert(const GameObject2D& obj) {

			for (std::vector<std::shared_ptr<Cell>>::iterator cell = m_Cell.begin(), end = m_Cell.end(); cell != end; ++cell) {
				if ((*cell)->IsEmpty()) {

					(*cell)->AddElement(obj);
					break;

				}
			}

		}
		void Grid::Erase(const GameObject2D& obj) {
			// Need optimalization: remove element at specific index and dont iterate through whole table !!!

			for (std::vector<std::shared_ptr<Cell>>::iterator cell = m_Cell.begin(), end = m_Cell.end(); cell != end; ++cell) {
				if (!(*cell)->IsEmpty()) {
					if ((*cell)->GetElement()->GetObject() == &obj) {

						(*cell)->RemoveElement();
						break;

					}
				}
			}

		}
		void Grid::Erase(unsigned int index) {

			if (index <= m_Cell.size()) {
				Cell& t_Cell = *m_Cell.at(index);

				if (!t_Cell.IsEmpty()) {
					t_Cell.RemoveElement();
				}
			}

		}
		void Grid::Show(void) {

			if (m_ShowState == false) {
				m_ShowState = true;

				for (std::vector<std::shared_ptr<Cell>>::iterator cell = m_Cell.begin(), end = m_Cell.end();
					cell != end;
					++cell)
				{
					(*cell)->Show();
				}
			}

		}
		void Grid::Hide(void) {

			if (m_ShowState == true) {
				m_ShowState = false;

				for (std::vector<std::shared_ptr<Cell>>::iterator cell = m_Cell.begin(), end = m_Cell.end();
					cell != end;
					++cell)
				{
					(*cell)->Hide();
				}
			}

		}
		void Grid::Draw() const {

			// Draw table
			Strave::StraveApplication->GetWindow()->Draw<sf::RectangleShape>(*this->GetModel<sf::RectangleShape>());

			// Cycle whole vector of cells
			for (std::vector<std::shared_ptr<Cell>>::iterator iter = const_cast<std::vector<std::shared_ptr<Cell>>&>(m_Cell).begin(), 
				end = const_cast<std::vector<std::shared_ptr<Cell>>&>(m_Cell).end();
				iter != end;
				++iter)
			{
				// Draw each cell
				Strave::StraveApplication->GetWindow()->Draw<sf::RectangleShape>(*(*iter)->GetModel<sf::RectangleShape>());
			
				// Draw each cells element if it contains any
				if (!(*iter)->IsEmpty()) {
					Strave::StraveApplication->GetWindow()->Draw<sf::RectangleShape>(*(*iter)->GetElement()->GetModel<sf::RectangleShape>());
				}
			}

		}
		bool Grid::MouseDrag(void) {

			if (m_ShowState == true) {
				bool t_Dragable = m_Mouse->Drag(*this);

				if (t_Dragable) {
					this->SetGridPosition(this->GetPosition());
					return t_Dragable;
				}
			}

			return false;

		}

		//========================================================================================
		//					class: TextBox
		//========================================================================================

		TextBox::TextBox()
			: UserInterface()
		{}
		TextBox::~TextBox() {}

		//========================================================================================
		//					class: Label
		//========================================================================================

		Label::Label()
			: UserInterface()
		{}
		Label::~Label() {}

		//========================================================================================
		//					class: Button
		//========================================================================================

		Button::Button()
			: UserInterface()
		{}
		Button::Button(std::weak_ptr<sf::Texture> texture, std::weak_ptr<sf::Sprite> sprite, std::weak_ptr<sf::RectangleShape> rectSprite, const Strave::Vector2f& position, const Strave::Vector2f& spriteSize, std::string title)
			: UserInterface(OBJECT_TYPE::UI_BUTTON, texture, sprite, rectSprite, position, spriteSize)
		{}
		Button::~Button() {}

		Button* Button::Create(sf::Texture& texture, const Strave::Vector2f& position, const Strave::Vector2f& size, std::string title) {

			std::shared_ptr<sf::Sprite> t_Sprite2D = std::make_shared<sf::Sprite>();
			std::shared_ptr<sf::RectangleShape> t_RectSprite2D = std::make_shared<sf::RectangleShape>();


			t_Sprite2D->setTexture(texture);
			t_Sprite2D->setScale(Convert::ToVector2(size));
			t_Sprite2D->setPosition(Convert::ToVector2(position));

			t_RectSprite2D->setTexture(&texture);
			t_RectSprite2D->setSize(Convert::ToVector2(size));
			t_RectSprite2D->setPosition(Convert::ToVector2(position));

			return new Button(std::make_shared<sf::Texture>(texture), t_Sprite2D, t_RectSprite2D, position, size, title);

		}

	}

}

