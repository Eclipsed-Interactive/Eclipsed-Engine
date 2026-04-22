#pragma once

#include <array>

namespace Eclipse
{
	template<typename T, size_t capacity>
	class RingBuffer final
	{
	public:
		void Push(const T& value);
		
		const T& Get(size_t i)const;
		T& Get(size_t i);

		size_t Count() const;
		void Clear();

		std::array<T, capacity>& GetBuffer();

	private:
		std::array<T, capacity> buffer{};
		size_t index = 0;
		size_t size = 0;
	};
}

#include "RingBuffer.inl"