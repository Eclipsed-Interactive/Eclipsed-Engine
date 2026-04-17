#include "pch.h"

#include "FileUtilities.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <shobjidl.h>
#include <string>

std::string Eclipse::Files::SelectFolderDialog()
{
    std::string result;

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr)) {
        IFileDialog* pFileDialog = nullptr;
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pFileDialog));

        if (SUCCEEDED(hr)) {
            DWORD options = 0;
            pFileDialog->GetOptions(&options);
            pFileDialog->SetOptions(options | FOS_PICKFOLDERS); // Folder mode

            hr = pFileDialog->Show(NULL);
            if (SUCCEEDED(hr)) {
                IShellItem* pItem = nullptr;
                hr = pFileDialog->GetResult(&pItem);
                if (SUCCEEDED(hr)) {
                    PWSTR pszFilePath = NULL;
                    pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Convert from wide string (UTF-16) to UTF-8 std::string
                    if (pszFilePath) {
                        int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, NULL, 0, NULL, NULL);
                        std::string path(sizeNeeded - 1, 0); // exclude null terminator
                        WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, &path[0], sizeNeeded, NULL, NULL);
                        result = path;

                        CoTaskMemFree(pszFilePath);
                    }

                    pItem->Release();
                }
            }
            pFileDialog->Release();
        }
        CoUninitialize();
    }

    return result; // empty if cancelled
}

std::string Eclipse::Files::SelectFileDialog()
{
    std::string result;

    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileDialog* pFileDialog = nullptr;
        hr = CoCreateInstance(
            CLSID_FileOpenDialog,
            nullptr,
            CLSCTX_ALL,
            IID_PPV_ARGS(&pFileDialog)
        );

        if (SUCCEEDED(hr))
        {
            DWORD options = 0;
            pFileDialog->GetOptions(&options);

            // File selection mode (default)
            pFileDialog->SetOptions(options | FOS_FILEMUSTEXIST);

            hr = pFileDialog->Show(nullptr);
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem = nullptr;
                hr = pFileDialog->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath = nullptr;
                    pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    if (pszFilePath)
                    {
                        int sizeNeeded = WideCharToMultiByte(
                            CP_UTF8, 0,
                            pszFilePath, -1,
                            nullptr, 0,
                            nullptr, nullptr
                        );

                        std::string path(sizeNeeded - 1, 0);
                        WideCharToMultiByte(
                            CP_UTF8, 0,
                            pszFilePath, -1,
                            &path[0], sizeNeeded,
                            nullptr, nullptr
                        );

                        result = path;
                        CoTaskMemFree(pszFilePath);
                    }

                    pItem->Release();
                }
            }

            pFileDialog->Release();
        }

        CoUninitialize();
    }

    return result; // empty if cancelled
}

std::string Eclipse::Files::SaveFileDialog()
{
    std::string result;

    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileSaveDialog* pFileDialog = nullptr;
        hr = CoCreateInstance(
            CLSID_FileSaveDialog,
            nullptr,
            CLSCTX_ALL,
            IID_PPV_ARGS(&pFileDialog)
        );

        if (SUCCEEDED(hr))
        {
            DWORD options = 0;
            pFileDialog->GetOptions(&options);

            // Allow creating new files, warn on overwrite
            pFileDialog->SetOptions(options | FOS_OVERWRITEPROMPT);

            // Optional filters
            COMDLG_FILTERSPEC filters[] =
            {
                { L"Text File (*.txt)", L"*.txt" },
                { L"All Files (*.*)", L"*.*" }
            };
            pFileDialog->SetFileTypes(_countof(filters), filters);
            pFileDialog->SetFileTypeIndex(1);

            hr = pFileDialog->Show(nullptr);
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem = nullptr;
                hr = pFileDialog->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath = nullptr;
                    pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    if (pszFilePath)
                    {
                        int sizeNeeded = WideCharToMultiByte(
                            CP_UTF8, 0,
                            pszFilePath, -1,
                            nullptr, 0,
                            nullptr, nullptr
                        );

                        std::string path(sizeNeeded - 1, 0);
                        WideCharToMultiByte(
                            CP_UTF8, 0,
                            pszFilePath, -1,
                            &path[0], sizeNeeded,
                            nullptr, nullptr
                        );

                        result = path;
                        CoTaskMemFree(pszFilePath);
                    }

                    pItem->Release();
                }
            }

            pFileDialog->Release();
        }

        CoUninitialize();
    }

    return result; // path only — file not created yet
}


