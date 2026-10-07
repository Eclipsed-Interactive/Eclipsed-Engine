#include "Arena.h"

#include "Core/Standard/Core/Utility.hpp"
#include "Core/Standard/Core/Asserts.hpp"

namespace Eclipse
{
	Arena::Arena()
	{
		myOffset = 0;
		myBytes = nullptr;
		myCapacity = 0;
	}

	Arena::Arena(Size size)
	{
		delete[] myBytes;
	}

	Arena::~Arena()
	{
		delete[] myBytes;
	}

	void Arena::Resize(Size size)
	{
		if (myCapacity != 0)
		{
			delete[] myBytes;
		}

		myBytes = new Byte[size];
		myOffset = 0;
		myCapacity = size;
	}

	void* Arena::Allocate(Size size, Size align)
	{
		ASSERT(IsPowerOf2(align));

		Byte* current = myBytes + myOffset;

		const Size address = reinterpret_cast<Size>(current);
		const Size aligned = (address + align - 1) & ~(align - 1);

		const Size padding = aligned - address;

		ASSERT(myOffset + padding + size <= myCapacity);

		Byte* memory = current + padding;

		myOffset += padding + size;
		return memory;
	}

	void Arena::Reset()
	{
		myOffset = 0;
		MemSet(myBytes, 0, myCapacity);
	}

	Size Arena::Capacity()
	{
		return myCapacity;
	}

	Size Arena::Used()
	{
		return myOffset;
	}

	Size Arena::Remaining()
	{
		return myCapacity - myOffset;
	}
}