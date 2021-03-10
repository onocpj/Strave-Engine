#include "BasicContainer.h"

#include "../datatypes/UDT.h"
#include "../logger/Log.h"

namespace Strave
{

	template<typename Type>
	Type* BasicContainer<Type>::GetNext(const Uint64 currentKey) const
	{
		Uint64 next = currentKey + 1;

		if (this->IsKeyInRange(next))
			return this->GetElement(next);
		else
			return UNDEF_PTR;
	}

	template<typename Type>
	Type* BasicContainer<Type>::GetElement(const Uint64 key) const
	{
		return const_cast<Type*>(m_Container.at(key));
	}

	template<typename Type>
	bool BasicContainer<Type>::IsEmpty(void) const
	{
		if (m_Container.size() == 0)
		{
			return BC_EMPTY;

		}
		else {
			return !BC_EMPTY;
		}
	}

	template<typename Type>
	bool BasicContainer<Type>::Contains(const Type& obj) const
	{
		if (m_Container.size() > 0) {
			for (auto iter = m_Container.begin(), end = m_Container.end(); iter != end; ++iter) {
				if (iter->second == &obj) {
					return true;
				}
			}
		}

		return false;
	}

	template<typename Type>
	Int64 BasicContainer<Type>::InsertToContainer(const Type& obj)
	{
		Int64 assignedPK = m_PK.AssignPK(*this, obj);

		if (assignedPK != BC_UNDEF_KEY)
		{
			m_Container.insert(std::pair<Uint64, const Type*>((Uint64)assignedPK, &obj));
			SV_CORE_INFO("function const Uint64& BasicContainer<Type>::PushToContainer(const Type& obj): element pushed to container");

		}
		else {
			SV_CORE_ERROR("function const Uint64& BasicContainer<Type>::PushToContainer(const Type& obj): unable to push element to container");
		}

		return assignedPK;
	}

	template<typename Type>
	Int64 BasicContainer<Type>::InsertToContainerRecycled(const Type& obj)
	{
		Int64 assignedPK = BC_UNDEF_KEY;

		if (!IsKeyHolderEmpty())
		{
			Uint64 keyFromHolder = GetKeyFromHolder();
			auto element = m_Container.find(keyFromHolder);

			element->second = &obj;
			assignedPK = keyFromHolder;

			SV_CORE_INFO("function const Uint64& BasicContainer<Type>::PushToContainer(const Type& obj): element pushed to container");

		}
		else {
			assignedPK = BasicContainer::InsertToContainer(obj);
			SV_CORE_ERROR("function const Uint64& BasicContainer<Type>::PushToContainer(const Type& obj): unable to push element to container");
		}

		return assignedPK;
	}

	template<typename Type>
	void BasicContainer<Type>::EraseFromContainer(const Uint64 key)
	{
		if (m_PK.IsInRange(key))
		{
			auto element = m_Container.find(key);
			PassKeyToHolder(key);

			element->second = UNDEF_PTR;

			SV_CORE_INFO("function void BasicContainer<Type>::EraseFromContainer(const Uint64 key): element erased");

		} else {
			SV_CORE_ERROR("function void BasicContainer<Type>::EraseFromContainer(const Uint64 key): key out of range");
		}
	}

	template<typename Type>
	void BasicContainer<Type>::PassKeyToHolder(Uint64 key)
	{
		m_EmptyKeyHolder.push(key);
	}

	template<typename Type>
	Int64 BasicContainer<Type>::GetKeyFromHolder(void)
	{
		if (!IsKeyHolderEmpty())
		{
			Int64 popedKey = (Int64)m_EmptyKeyHolder.top();
			m_EmptyKeyHolder.pop();

			return popedKey;

		}
		else {
			return BC_UNDEF_KEY;
		}
	}

	template<typename Type>
	bool BasicContainer<Type>::IsKeyHolderEmpty(void)
	{
		return m_EmptyKeyHolder.size() > 0 ? false : true;
	}

	template<typename Type>
	bool BasicContainer<Type>::IsKeyInRange(Uint64 key)
	{
		return key < this->GetSize() ? true : false;
	}

	//=============================================================================
	//								class: PrimaryKey
	//=============================================================================
	template<typename Type>
	Int64 BasicContainer<Type>::PrimaryKey::AssignPK(BasicContainer& bc, const Type& obj)
	{
		if (!bc.Contains(obj))
		{
			return ++identifier;
		}

		return BC_UNDEF_KEY;
	}

	template<typename Type>
	bool BasicContainer<Type>::PrimaryKey::IsInRange(Uint64 key)
	{
		return key <= identifier ? true : false;
	}
}
