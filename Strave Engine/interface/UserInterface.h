#pragma once

#include "../entities/GameObject2D.h"
#include "../input/Keyboard.h"
#include "../input/Mouse.h"
#include "../core/datatypes/DT.h"

#include <vector>
#include <memory>


#define GUI_UINT_BEGINING			0
#define GUI_INIT_DEF_VIEW_STATE		1
#define GUI_NO_MARGINS				{ 0.0f, 0.0f }
#define GUI_VECTOR_BEGINING			GUI_UINT_BEGINING

#define GUI_ELEMENT_MARGINS_PERCENTAGE		85.0f


namespace Strave {

	// Predeclared classes 
	class WindowsWindow;
	class Application;

	namespace GUI {

		class SV_SANDBOX_API UserInterface abstract : public GameObject2D {
		private:
			friend Application;

		public:
			UserInterface();
			UserInterface(OBJECT_TYPE objType, std::weak_ptr<sf::Texture> texture, std::weak_ptr<sf::Sprite> sprite, std::weak_ptr<sf::RectangleShape> rectSprite, const Strave::Vector2f& position, const Strave::Vector2f& spriteScale);
			virtual ~UserInterface();

			/// \brief Returns show state of object on screen
			///
			/// \return True if object show state is true, false otherwise
			/// 
			inline bool GetShowState(void) const { 
				
				return this->m_ShowState; 
			
			}
			/// \brief Show object on screen
			/// 
			inline virtual void Show(void) { 
				
				if (this->m_ShowState == false) {
					this->m_ShowState = true;
				}
			
			}
			/// \brief Hide object on scene
			/// 
			inline virtual void Hide(void) { 
				
				if (this->m_ShowState == true) {
					this->m_ShowState = false;
				}
			
			}
			/// \brief Draw object on screen 
			/// Warning: drawing is handled by engine, not recomended using this function
			/// 
			virtual void Draw(void) const;
			/// \brief Chceck if object is clicked
			///
			/// \param button - button to check
			///
			/// \return True if the object is clicked, false otherwise
			/// 
			virtual bool MouseClick(Mouse::Button button) const;
			/// \brief Chceck if object is double clicked
			///
			/// \param button - button to check
			///
			/// \return True if the object is double clicked, false otherwise
			/// 
			virtual bool MouseDoubleClick(Mouse::Button button) const;
			/// \brief Chceck if object is double clicked in specific delay
			///
			/// \param button - button to check
			/// \param clickDelay - delay to second click
			///
			/// \return True if the object is double clicked in specific delay, false otherwise
			/// 
			virtual bool MouseDoubleClick(Mouse::Button button, float clickDelay) const;
			/// \brief Chceck if object was clicked and released
			///
			/// \param button - button to check
			///
			/// \return True if the object was clicked and released, false otherwise
			/// 
			virtual bool MouseReleased(Mouse::Button button) const;
			/// \brief Chceck if object is mouse holded
			///
			/// \param button - button to check
			///
			/// \return True if the object is mouse holded, false otherwise
			/// 
			virtual bool MouseHold(Mouse::Button button) const;
			/// \brief Check if object is mouse hovered
			///
			/// \return True if the object is mouse hovered, false otherwise
			/// 
			virtual bool MouseHover(void);
			/// \brief Eables to drag object with the mouse
			/// Also checks if object is dragged by mouse
			///
			/// \return True if object is dragged by mouse, false otherwise
			/// 
			virtual bool MouseDrag(void);

		private:
			void Init(WindowsWindow& window);

		protected:
			std::shared_ptr<Mouse>		m_Mouse;
			bool						m_ShowState;
			const sf::RenderWindow*		m_WindowReference;

		};
		class Cell : public UserInterface {
		private:
			class Element : public GameObject2D {

			public:
				~Element();

				inline const GameObject2D* GetObject(void) const {

					return m_GO2DReference;

				}
				static Element* Create(const Cell& cell);
				void Add(const GameObject2D& obj);
				void Remove(void);

			private:
				Element(std::weak_ptr<sf::Texture> texture, std::weak_ptr<sf::Sprite> sprite, std::weak_ptr<sf::RectangleShape> rectSprite, const Strave::Vector2f& position, const Strave::Vector2f& spriteSize);
				Element();

				const GameObject2D* m_GO2DReference;

			};
			friend GameObject2D;

		public:
			inline ~Cell() {

				delete m_CellElement;

			}

			/// \brief Returns element of cell
			///
			/// \return Reference to element of cell
			/// 
			inline Element* GetElement(void) const {

				return m_CellElement;

			}
			static Cell* Create(sf::Texture& texture, const Strave::Vector2f& position, const Strave::Vector2f& spriteSize = GAME_OBJECT_2D_SCALE);
			/// \brief Inserts element to cell
			///
			/// \param obj - element to insert
			/// 
			void AddElement(const GameObject2D& obj) const;
			/// \brief Removes element from cell
			/// 
			void RemoveElement(void) const;
			/// \brief Chcecks if cell is containing any object
			///
			/// \return True if cell is containing any object, False otherwise
			/// 
			inline bool IsEmpty(void) const { 
			
				if (m_CellElement->GetObject() == UNDEF_PTR) {
					return true;

				} else {
					return false;
				}
			
			};
			/// \brief Draw object on screen 
			/// Warning: drawing is handled by engine, not recomended using this function
			/// 
			virtual void Draw(void) const override;
		
		private:
			Cell(std::weak_ptr<sf::Texture> texture, std::weak_ptr<sf::Sprite> sprite, std::weak_ptr<sf::RectangleShape> rectSprite, const Strave::Vector2f& position, const Strave::Vector2f& spriteSize);
			Cell();

			Element* m_CellElement;

		};
		class Grid : public UserInterface {
		public:
			inline ~Grid() {}

			static Grid* Create(const Strave::Vector2u& dimensions, sf::Texture& gridTexture, sf::Texture& cellTexture, const Strave::Vector2f& position, const Strave::Vector2f& spriteSize = GAME_OBJECT_2D_SCALE);
			/// \brief Returns table size
			///
			/// \return Size of table
			/// 
			inline unsigned int GetSize(void) const { 
				
				return m_Cell.size(); 
			
			}
			/// \brief Returns specific cell
			///
			/// \return Reference to specific cell
			/// 
			inline const Cell& GetCell(unsigned int index) const { 

				return *m_Cell.at(index); 

			}
			/// \brief Returns pointer to object instance of cell if its containing any
			///
			/// \param cell - pointer to object of this cell will be returned
			/// \return Pointer to object instance of cell if containing any, UNDEF_PTR(empty) otherwise
			/// 
			inline const GameObject2D* GetObject(const Cell& cell);
			/// \brief Returns pointer to object instance of cell if its containing any
			///
			/// \param index - index of cell whose object instance will be returned
			/// \return Pointer to object instance of cell if containing any, UNDEF_PTR(empty) otherwise
			/// 
			inline const GameObject2D* GetObject(unsigned int index);
			/// \brief Sets margins of columns, relative to table
			///
			/// \param margins - margins size
			/// 
			void SetMargins(const Strave::Vector2f& margins);
			/// \brief Sets table position
			///
			/// \param position - position of table
			/// 
			void SetGridPosition(const Strave::Vector2f& position);
			/// \brief Insert element to table
			///
			/// \param obj - element to insert
			/// 
			void Insert(const GameObject2D& obj);
			/// \brief Erase element from table if containing it
			///
			/// \param obj - element to remove
			/// 
			void Erase(const GameObject2D& obj);
			/// \brief Erase element from table at specific index if containing any
			///
			/// \param index - index of element to remove
			/// 
			void Erase(unsigned int index);
			/// \brief Show object on screen
			/// 
			virtual void Show(void) override;
			/// \brief Hide object from screen
			/// 
			virtual void Hide(void) override;
			/// \brief Draw object on screen 
			/// Warning: drawing is handled by engine, not recomended using this function
			/// 
			virtual void Draw(void) const override;
			/// \brief Check if the object is draged by mouse
			///
			/// \return True if object is dragged by mouse, false otherwise
			/// 
			virtual bool MouseDrag(void) override;

		protected:
			Grid(const Strave::Vector2u& dimensions, sf::Texture& cellTexture, std::weak_ptr<sf::Texture> texture, std::weak_ptr<sf::Sprite> sprite, std::weak_ptr<sf::RectangleShape> rectSprite, const Strave::Vector2f& position, const Strave::Vector2f& spriteSize);
			Grid();

		private:
			void AlignCells(unsigned int rowsNumber, unsigned int columnsNumber, sf::Texture& cellTexture, const Strave::Vector2f& tablePosition, const Strave::Vector2f& tableSize);
			GameObject2D::SetPosition;


			unsigned int						m_RowsNumber;
			unsigned int						m_ColumnsNumber;
			std::vector<std::shared_ptr<Cell>>	m_Cell;
			Strave::Vector2f					m_MarginsSize;

		};
		class TextBox : public UserInterface {
		public:
			TextBox();
			~TextBox();

		private:


		};
		class Label : public UserInterface {
		public:
			Label();
			~Label();
			
		private:


		};
		class Button : public UserInterface {
		public:
			~Button();

			static Button* Create(sf::Texture& texture, const Strave::Vector2f& position, const Strave::Vector2f& size, std::string title = "");

		private:
			Button();
			Button(std::weak_ptr<sf::Texture> texture, std::weak_ptr<sf::Sprite> sprite, std::weak_ptr<sf::RectangleShape> rectSprite, const Strave::Vector2f& position, const Strave::Vector2f& spriteSize, std::string title);

			std::string m_Title;

		};

	}

}

