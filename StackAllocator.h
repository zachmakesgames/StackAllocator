#pragma once
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//typedef StackMarker as void* to
//make marking the stack super simple
typedef void* StackMarker;

class StackAllocator
{
	protected:

		//The memory allocated by the OS to work on
		void* memory = nullptr;

		//The top of the stack
		void* free_marker = nullptr;

		//The total size of the stack
		size_t stack_size;

	public:
		inline StackAllocator(size_t StackSize) {

			//Set the size
			this->stack_size = StackSize;

			//Allocate the raw memory
			this->memory = malloc(this->stack_size);

			//Set the top of stack marker to the bottom of the memory
			this->free_marker = this->memory;
		}


		inline ~StackAllocator() {
			//Free the memory automagically
			delete[] this->memory;
		}

		//Allocate some memory from the stack
		inline void* Alloc(size_t size, bool ZeroOut = false) {

			//calculate the new end of the stack
			uintptr_t end = (uintptr_t)free_marker + size;

			//If the end of the requested block would overflow the
			//stack size then return nullptr
			if (end > (uintptr_t)memory + stack_size) {
				return nullptr;
			}

			//get a copy of the current top of stack to return
			//to the user and the allocated block
			void* alloc_block = free_marker;

			//set the top of stack marker to the new top of stack
			//calculated earlier
			free_marker = (void*)end;

			//if ZeroOut is true then zero out the allocated block
			if (ZeroOut) {
				memset(alloc_block, 0x00, size);
			}

			//return the allocated memory
			return alloc_block;
		}


		//Get a stack marker for easy rollback
		inline StackMarker MarkStack() {
			//returns the current top of stack casted to a StackMarker
			return (StackMarker)this->free_marker;
		}


		//Roll the stack back to the given stack marker
		//If ZeroOut is true the rolled back memory is also zeroed out
		inline void RollBackStack(StackMarker Marker, bool ZeroOut = false) {

			//calculate the size of memory to wipe out by subtracting the marker
			//from the current top of the stack
			size_t wipe_size = (uintptr_t)this->free_marker - (uintptr_t)Marker;

			//Then set the top of the stack back to the given marker
			this->free_marker = (void*)Marker;

			//if ZeroOut is true then zero out the memory
			if (ZeroOut) {
				memset(this->free_marker, 0x00, wipe_size);
			}
		}


		//Wipe the entire stack
		//WARNING: ZeroOut is default TRUE unlike other functions
		//We want to always zero out a wiped stack by default
		//because it won't happen as often as rolling back.
		inline void WipeStack(bool ZeroOut = true) {

			//Set the top of stack back to the bottom of the allocated memory
			this->free_marker = this->memory;

			//Zero out the memory if requested
			if (ZeroOut) {
				memset(this->memory, 0x00, this->stack_size);
			}
		}
};

