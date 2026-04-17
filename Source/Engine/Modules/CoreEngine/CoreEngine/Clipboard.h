#pragma once

namespace Eclipse
{
    class ClipBoard
    {
    public:
        static void CopyToClipboard(const void* aData, size_t aDataSize);
        static void* GetClipboardData();
    };
}