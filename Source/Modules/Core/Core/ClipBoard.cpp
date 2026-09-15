#include "pch.h"

#include "Clipboard.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Eclipse
{
    void ClipBoard::CopyToClipboard(const void* aData, size_t aDataSize)
    {
        OpenClipboard(NULL);
        EmptyClipboard();

        LPTSTR copiedData;
        HGLOBAL bufferHandle = GlobalAlloc(GMEM_MOVEABLE, aDataSize + 1);
        if (bufferHandle == NULL)
        {
            CloseClipboard();
            return;
        }
        copiedData = (LPTSTR)GlobalLock(bufferHandle);
        memcpy(copiedData, aData, aDataSize);
        copiedData[aDataSize] = (TCHAR)0;
        GlobalUnlock(bufferHandle);

        SetClipboardData(CF_TEXT, bufferHandle);

        CloseClipboard();
    }

    void* ClipBoard::GetClipboardData()
    {
        HGLOBAL bufferHandle;

        OpenClipboard(NULL);
        bufferHandle = ::GetClipboardData(CF_TEXT);
        if (bufferHandle == NULL)
        {
            return nullptr;
            CloseClipboard();
        }

        LPTSTR copiedData;
        copiedData = (LPTSTR)GlobalLock(bufferHandle);
        if (copiedData == NULL)
        {
            CloseClipboard();
            return nullptr;
        }

        GlobalUnlock(bufferHandle);
        
        CloseClipboard();

        return copiedData;
    }
}