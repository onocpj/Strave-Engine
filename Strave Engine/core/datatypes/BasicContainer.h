#pragma once

#include <map>
#include <stack>

#include "../datatypes/DT.h"

#define BC_EMPTY true
#define BC_UNDEF_KEY -1

namespace Strave
{
	template<typename Type>
	class BasicContainer
	{
	private:
		class PrimaryKey
		{
		public:
			inline PrimaryKey() : identifier(BC_UNDEF_KEY) {}
			Int64 AssignPK(BasicContainer& bc, const Type& obj);
			bool IsInRange(Uint64 key);

			Uint64 identifier;
		};
		using PK = PrimaryKey;

	public:
		inline virtual ~BasicContainer() {}

		inline Int64 GetSize(void) const
		{
			return m_Container.size();
		}
		Type* GetNext(const Uint64 currentKey) const;
		Type* GetElement(const Uint64 key) const;
		bool IsEmpty(void) const;
		bool Contains(const Type& obj) const;
		Int64 InsertToContainer(const Type& obj);
		Int64 InsertToContainerRecycled(const Type& obj);
		void EraseFromContainer(const Uint64 key);

	private:
		void PassKeyToHolder(Uint64 key);
		Int64 GetKeyFromHolder(void);
		bool IsKeyHolderEmpty(void);
		bool IsKeyInRange(Uint64 key);

	private:
		std::map<Uint64, const Type*>	m_Container;
		std::stack<Uint64>				m_EmptyKeyHolder;

	protected:
		PrimaryKey						m_PK;
	};
}
