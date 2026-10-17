#pragma once

namespace Eclipse::Hash
{
	inline constexpr unsigned long long FNV1A(const char* str)
	{
		unsigned long long hash = 14695981039346656037ull;

		while (*str)
		{
			hash ^= static_cast<unsigned long long>(*str++);
			hash *= 1099511628211ull;
		}

		return hash;
	}

	inline constexpr int FNV1A32(const char* str)
	{
		int hash = 2166136261;

		while (*str)
		{
			hash ^= static_cast<int>(*str++);
			hash *= 16777619;
		}

		return hash;
	}

	inline constexpr unsigned int FNV1A_U32(const char* str)
	{
		unsigned int hash = 2166136261u;

		while (*str)
		{
			hash ^= static_cast<unsigned int>(*str++);
			hash *= 16777619u;
		}

		return hash;
	}
}