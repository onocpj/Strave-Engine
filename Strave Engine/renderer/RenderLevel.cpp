#include "RenderLevel.h"

#include "../entities/GameObject2D.h"
#include "../core/datatypes/DT.h"
#include "../core/datatypes/UDT.h"
#include "../core/additive/Additive.h"
#include "../core/datatypes/Vector2.h"

#include <map>
#include <algorithm>


using namespace Strave::Additive;


namespace Strave 
{
	const void RenderLevel::SortObjectsByLevel(const std::vector<GameObject2D*>& objects)
	{
		Uint64 minIndex = UNDEF_UINT;

		for (Uint64 i = 0; i != objects.size(); i++)
		{
			minIndex = i;
			for (Uint64 j = i + 1; j != objects.size(); j++)
			{
				if ((Uint16)objects[j]->GetPosition().y + (Uint16)objects[j]->GetSpriteSize().y <
					(Uint16)objects[minIndex]->GetPosition().y + (Uint16)objects[minIndex]->GetSpriteSize().y
				) 
				{
					minIndex = j;
				}
			}

			std::swap(const_cast<GameObject2D*&>(objects[minIndex]), const_cast<GameObject2D*&>(objects[i]));
		}
	}
}
