#pragma once
 
#include <filesystem>
#include <string>
#include "EclipsedEngine/Plugin/Plugin.h"

namespace Eclipse
{
	class GameModuleManager
	{
    public:
        static bool Load(const char* source);
        static void Unload();

        static bool Reload();

        [[nodiscard]]
        static bool IsLoaded();

    private:
        static Plugin* game;
        static std::string name;
        static std::string sourcepath;
	};
}