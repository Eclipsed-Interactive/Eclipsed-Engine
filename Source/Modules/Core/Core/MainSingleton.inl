namespace Eclipse
{

    template<typename T>
    void MainSingleton::AddInstance(T& ref)
    {
        constexpr TypeID type = Type::GetTypeID<T>();

        if (mySingletons.contains(type))
            return;

        SingletonEntry entry;

        entry.instance = &ref;
        entry.deleter = nullptr;

        mySingletons[type] = entry;
    }


    template<typename T>
    void MainSingleton::AddInstance(T* ptr, bool destroy)
    {
        constexpr TypeID type = Type::GetTypeID<T>();

        if (mySingletons.contains(type))
            return;

        SingletonEntry entry;

        entry.instance = ptr;

        if (destroy)
        {
            entry.deleter = [](void* p)
                {
                    delete static_cast<T*>(p);
                };
        }
        else
        {
            entry.deleter = nullptr;
        }

        mySingletons[type] = entry;
    }


    template<typename T>
    T& MainSingleton::GetInstance()
    {
        constexpr TypeID type = Type::GetTypeID<T>();

        auto it = mySingletons.find(type);
        if (it == mySingletons.end())
            throw std::runtime_error("Singleton not registered.");

        return *static_cast<T*>(it->second.instance);
    }


    template<typename T>
    T* MainSingleton::GetPointer()
    {
        constexpr TypeID type = Type::GetTypeID<T>();

        auto it = mySingletons.find(type);
        if (it == mySingletons.end())
            throw std::runtime_error("Singleton not registered.");

        return static_cast<T*>(it->second.instance);
    }


    template<typename T, typename... Args>
    T& MainSingleton::RegisterInstance(Args&&... args)
    {
        constexpr TypeID type = Type::GetTypeID<T>();

        if (mySingletons.contains(type))
            return GetInstance<T>();

        T* object = new T(std::forward<Args>(args)...);

        SingletonEntry entry;

        entry.instance = object;

        entry.deleter = [](void* p)
            {
                delete static_cast<T*>(p);
            };

        mySingletons[type] = entry;

        return *object;
    }


    template<typename T>
    bool MainSingleton::Exists()
    {
        constexpr TypeID type = Type::GetTypeID<T>();
        return mySingletons.contains(type);
    }


    inline void MainSingleton::Destroy()
    {
        for (auto& [type, entry] : mySingletons)
        {
            if (entry.deleter)
                entry.deleter(entry.instance);
        }

        mySingletons.clear();
    }


    template<typename T>
    inline void MainSingleton::RemoveInstance()
    {
        constexpr TypeID type = Type::GetTypeID<T>();
        auto it = mySingletons.find(type);

        if (it == mySingletons.end())
            return;

        SingletonEntry& entry = it->second;

        if (entry.deleter)
            entry.deleter(entry.instance);

        mySingletons.erase(it);
    }
}