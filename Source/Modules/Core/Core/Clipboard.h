#pragma once

#include "Core.Core.hpp"

namespace Eclipse
{
    class CORE_API ClipBoard
    {
    public:
        static void CopyToClipboard(const void* aData, size_t aDataSize);
        static void* GetClipboardData();
    };
}