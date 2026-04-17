#include "RingBuffer.h"

namespace Eclipse
{
	template<typename T, size_t capacity>
	inline void RingBuffer<T, capacity>::Push(const T& value)
	{
		buffer[index] = value;
		index = (index + 1) % capacity;

		if (size < capacity) size++;
	}

	template<typename T, size_t capacity>
	inline const T& RingBuffer<T, capacity>::Get(size_t i) const
	{
		return buffer[(index + capacity - size + i) % capacity];
	}
	
	template<typename T, size_t capacity>
	inline T& RingBuffer<T, capacity>::Get(size_t i)
	{
		return buffer[(index + capacity - size + i) % capacity];
	}

	template<typename T, size_t capacity>
	inline size_t RingBuffer<T, capacity>::Count() const
	{
		return size;
	}

	template<typename T, size_t capacity>
	inline void RingBuffer<T, capacity>::Clear()
	{
		for (size_t i = 0; i < size; i++)
		{
			buffer[i] = {};
		}

		size = 0;
		index = 0;
	}

	template<typename T, size_t capacity>
	inline std::array<T, capacity>& RingBuffer<T, capacity>::GetBuffer()
	{
		return buffer;
	}
}