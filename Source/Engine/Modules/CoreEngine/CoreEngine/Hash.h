#pragma once

#include <string>

namespace Eclipse::Hash
{
	inline size_t fnv1a_64(const std::string& aBytes)
	{
        uint64_t hash = 0xcbf29ce484222325ULL;
        for (unsigned char byte : aBytes)
        {
            hash ^= byte;
            hash *= 0x100000001b3ULL;
        }
        return hash;
	}
}