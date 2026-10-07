#pragma once

#include "Core/Standard/Core/Types.hpp"
#include "Core.Core.hpp"

namespace Eclipse
{
	// Allocates memory once, and then its managed by the owner.
	class CORE_API Arena final
	{
	public:
		explicit Arena(Size size);
		Arena();
		~Arena();

		Arena(const Arena&) = delete;
		Arena& operator=(const Arena&) = delete;

	public:
		void Resize(Size size);

	public:
		template<typename T>
		T* Allocate();

		void* Allocate(Size size, Size align);
		void Reset();

	public:
		Size Capacity();
		Size Used();
		Size Remaining();

	private:
		Byte* myBytes;
		Size myOffset;
		Size myCapacity;
	};

	template<typename T>
	inline T* Arena::Allocate()
	{
		return reinterpret_cast<T*>(
			Allocate(sizeof(T), alignof(T))
			);
	}
}