#include <iostream>
#include <chrono>
#include <assert.h>

#include "StackAllocator.h"

int main()
{
	
	int outter = 10000;
	int inner = 1000;

	std::cout << "Timing stack allocator for a " << outter << "x" << inner << " nested loop\n";

	auto start_time = std::chrono::high_resolution_clock::now();

	StackAllocator stack(sizeof(int)*outter * inner);

	for (int i = 0; i < outter; ++i) {
		for (int j = 0; j < inner; ++j) {
			StackMarker m = stack.MarkStack();

			int* x = (int*)stack.Alloc(sizeof(int));
			stack.RollBackStack(m, true);
		}
	}


	auto end_time = std::chrono::high_resolution_clock::now();

	auto delta = end_time - start_time;


	std::cout << "Timing with new and delete for an " << outter << "x" << inner << " nested loop\n";

	auto start_time2 = std::chrono::high_resolution_clock::now();



	for (int i = 0; i < outter; ++i) {
		for (int j = 0; j < inner; ++j) {
			
			int* x = new int;
			delete x;
		}
	}


	auto end_time2 = std::chrono::high_resolution_clock::now();

	auto delta2 = end_time2 - start_time2;

	
	std::cout << "Stack allocator time in ms: " << delta/std::chrono::milliseconds(1) << "\n";
	std::cout << "new time in ms: " << delta2 / std::chrono::milliseconds(1) << "\n";
}

