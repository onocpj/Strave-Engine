#pragma once

#include <queue>
#include <vector>
#include <algorithm>
#include <array>

#include "../Core.h"
#include "../datatypes/DT.h"
#include "../datatypes/UDT.h"

namespace Strave {

	template<typename Type> class Vector2;
	template<typename Type> class Vector4;

	namespace Additive {

		struct SV_SANDBOX_API ADDContainer {
			template<typename Type>	
			inline static void EraseElementsFromVector(std::vector<Type>& myVec, std::queue<unsigned int>& atIndex) {

				unsigned int t_VecSize;
				unsigned int t_Index;

				t_VecSize = (unsigned int)myVec.size();
				t_Index = 0;

				while (!atIndex.empty()) {

					t_Index = atIndex.front();
					atIndex.pop();

					if (t_VecSize <= t_Index) {
						myVec.erase(myVec.begin() + t_Index);
					}

				}

			}
			template<typename Type> 
			inline static void EraseElementsFromVector(std::vector<Type>& myVec, unsigned int atIndex) {

				unsigned int t_VecSize;

				t_VecSize = (unsigned int)myVec.size();

				if (t_VecSize <= atIndex) {
					myVec.erase(myVec.begin() + atIndex);
				}

			}
		};
		struct SV_SANDBOX_API Array abstract {
			template<typename Type> 
			inline static int CopyArray(const Type*& from, Type*& to, Uint64 size) {
					try {
						for (Uint64 index = 0; index != size; index++) {
							to[index] = from[index];
						}

						return 0;
					}
					catch (...) {
						return -1;
					}
			}
			template<typename Type, size_t size>
			inline static int CopyArray(const std::array<Type, size>& from, std::array<Type, size>& to)
			{
				try {
					to = from;
				}
				catch (...) {
					return -1;
				}
			}
			template<typename Type>
			inline static Type FindLargest(const Type*& from, Uint64 size) {
					Type largest = from[0];

					for (Uint64 index = 0; index != size; index++) {
						if (from[index] > largest) {
							largest = from[index];
						}
					}

					return largest;
			}
			template<typename Type>
			inline static void SelectionSort(std::vector<Type&>& arr)
			{
				Uint64 minIndex = 0;

				for (Uint64 i = 0; i != arr.size(); i++)
				{
					minIndex = i;
					for (Uint64 j = i + 1; j != arr.size(); j++)
					{
						if (arr[j] < arr[minIndex]) {
							minIndex = j;
						}
					}

					std::swap(arr[minIndex], arr[i]);
				}
			}
		};
		struct SV_SANDBOX_API Value abstract {
			template<typename Type>
			inline static void Swap(const Type* obj1, const Type* obj2) {
				const Type* temp = UNDEF_PTR;

				temp = obj1;
				obj1 = obj2;
				obj2 = obj1;
			}
		};

	}

}

