#pragma once

#include <string>




namespace Eclipse
{
	class MemoryMappedFile
	{
	public:
		MemoryMappedFile(const char* path);
		~MemoryMappedFile();

	public:
		void Open(const char* path);

		template<typename T>
		T* Get();

		template<typename T>
		const T* Get() const;

	private:
		void* data;
		void* fileHandle;
		void* mappedHandle;
		unsigned long long size;
	};

	template<typename T>
	inline T* MemoryMappedFile::Get()
	{
		return reinterpret_cast<T*>(data);
	}

	template<typename T>
	inline const T* MemoryMappedFile::Get() const
	{
		return reinterpret_cast<const T*>(data);
	}
}