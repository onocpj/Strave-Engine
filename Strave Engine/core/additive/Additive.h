#pragma once

#include <queue>
#include <vector>

#include "DT.h"

namespace Strave {

	template<typename Type> class Vector2;
	template<typename Type> class Vector4;

	namespace Additive {

		struct ADDContainer {
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
		struct Array abstract {
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
		};

	}

}

