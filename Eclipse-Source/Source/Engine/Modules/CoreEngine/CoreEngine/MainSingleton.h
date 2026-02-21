#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <stdexcept>

namespace Eclipse
{
    struct SingletonEntry
    {
        void* instance;
        void (*deleter)(void*);

        bool useDestructor = false;
    };

    class MainSingleton
    {
    public:
        template<typename T>
        static void AddInstance(T& ref, bool useDestructor = false);

        template<typename T>
        static T& GetInstance();

        template<typename T>
        static T GetRaw();

        template<typename T, typename ...Args>
        static T& RegisterInstance( bool useDestructor = false, Args&&... args);

        template<typename T>
        static bool Exists();

        static void Destroy();

    private:
        static inline std::unordered_map<std::type_index, SingletonEntry> mySingletons;
    };
}

#include "MainSingleton.inl"