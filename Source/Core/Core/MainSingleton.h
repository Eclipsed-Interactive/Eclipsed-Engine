#pragma once

#include "Core.Core.hpp"

#include <unordered_map>
#include <typeindex>
#include <utility>
#include <stdexcept>
#include <string>

namespace Eclipse
{

#ifdef ECL_EDITOR
    struct CORE_API EditorEntry
    {
        bool* IsPlaying;
        bool* IsPaused;
    };
#endif

    struct CORE_API SingletonEntry
    {
        void* instance = nullptr;
        void (*deleter)(void*) = nullptr;
    };


    class CORE_API MainSingleton
    {
    public:

        // Register existing instance (does NOT own it)
        template<typename T>
        static void AddInstance(T& ref);


        // Register pointer instance
        template<typename T>
        static void AddInstance(T* ptr, bool destroy = false);


        template<typename T>
        static T& GetInstance();


        template<typename T>
        static T* GetPointer();


        template<typename T, typename... Args>
        static T& RegisterInstance(Args&&... args);


        template<typename T>
        static bool Exists();


        static void Destroy();


    private:

        static std::unordered_map<std::string, SingletonEntry> mySingletons;
    };

}

#include "MainSingleton.inl"