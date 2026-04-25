#pragma once

#include <cstdint>

namespace Eclipse::Hashing
{
    inline unsigned long long fnv1a(const void* data, size_t size)
    {
        constexpr unsigned long long FNV_OFFSET_BASIS = 14695981039346656037ull;
        constexpr unsigned long long FNV_PRIME = 1099511628211ull;

        unsigned long long hash = FNV_OFFSET_BASIS;

        const unsigned long long* bytes = static_cast<const unsigned long long*>(data);

        for (unsigned long long i = 0; i < size; ++i)
        {
            hash ^= bytes[i];
            hash *= FNV_PRIME;
        }

        return hash;
    }
}