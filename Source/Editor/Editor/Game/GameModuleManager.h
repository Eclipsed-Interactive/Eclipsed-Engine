#pragma once
 
#include <filesystem>
#include <string>
#include "EclipsedEngine/Plugin/Plugin.h"

namespace Eclipse
{
    using Func = void(*)();

	class GameModuleManager
	{
    public:
        static bool TryCompileAndLoad();

        static bool Load();
        static void Unload();

        static bool Reload();

        [[nodiscard]]
        static bool IsLoaded();

        static bool HasChanged(const char* dllPath);
        static bool DllFileExists(const char* dllPath);

    private:
        static bool BuildDLL();
        static bool RunHeaderTool();
        static bool Compile();
        static bool GenerateCmake();

        static bool CopyDllFile();

    private:
        static void* gameModule;
        static Func initFunc;
        static Func cleanupFunc;

        static std::string name;
        static std::filesystem::path dllPath;
	};
}