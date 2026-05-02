#pragma once
#include <functional>
#include <cstddef>
#include <string>

namespace Eclipse::Assets
{
    struct GUID
    {
        unsigned long long high = 0;
        unsigned long long low = 0;

        bool operator==(const GUID& other) const
        {
            return high == other.high && low == other.low;
        }

        bool operator!=(const GUID& other) const
        {
            return !(*this == other);
        }

        static GUID GenerateGUID();
        void Generate();

        void FromString(const std::string& str);
        std::string ToString() const;
    };

    struct GUIDHash
    {
        std::size_t operator()(const GUID& guid) const
        {
            return std::hash<unsigned long long>()(guid.high) ^
                (std::hash<unsigned long long>()(guid.low) << 1);
        }
    };
}