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

        static bool HasChanged();
        static bool DllFileExists(const char* dllPath);

        [[nodiscard]] static bool HasBuildErrors() { return hasBuildErrors; }

    private:
        static bool RunBuildStep(const std::string& stepName, const std::filesystem::path& batchPath);

        static inline bool hasBuildErrors = false;

        static bool BuildDLL();
        static bool RunHeaderTool();
        static bool Compile();
        static bool GenerateCmake();

        static bool CopyDllFile();

    private:
        static void* gameModule;
        static Func registerReflectedTypesFunc;
        static Func cleanupFunc;

        static std::string name;
        static std::filesystem::path dllPath;
	};
}