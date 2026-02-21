#include "MainSingleton.h"

namespace Eclipse
{
    template<typename T>
    inline void MainSingleton::AddInstance(T& ref, bool useDestructor)
    {
        auto it = mySingletons.find(typeid(T));
        if (it == mySingletons.end())
        {
            SingletonEntry instance;

            instance.instance = ref;
            instance.useDestructor = useDestructor;
            instance.deleter = [](void* ptr) {delete static_cast<T*>(ptr);};

            mySingletons[typeid(T)] = instance;
        }
    }

    template<typename T>
    inline T& MainSingleton::GetInstance()
    {
        auto it = mySingletons.find(typeid(T));
        if (it != mySingletons.end())
            return *static_cast<T*>(it->second.instance);
        throw std::runtime_error("Singleton not registered.");
    }

    template<typename T>
    inline T MainSingleton::GetRaw()
    {
        auto it = mySingletons.find(typeid(T));
        if (it != mySingletons.end())
            return static_cast<T>(it->second.instance); // safe for raw pointers
        throw std::runtime_error("Singleton not registered.");
    }

    template<typename T, typename ...Args>
    inline T& MainSingleton::RegisterInstance(bool useDestructor, Args&&... args)
    {
        SingletonEntry instance;

        instance.instance = new T(std::forward<Args>(args)...);
        instance.useDestructor = useDestructor;
        instance.deleter = [](void* ptr) {delete static_cast<T*>(ptr);};

        mySingletons[typeid(T)] = instance;
        return *static_cast<T*>(instance.instance);
    }

    template<typename T>
    inline bool MainSingleton::Exists()
    {
        return mySingletons.find(typeid(T)) != mySingletons.end();
    }
}