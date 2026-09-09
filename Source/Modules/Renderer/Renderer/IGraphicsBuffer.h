#pragma once

#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include "Renderer.Core.hpp"

namespace Eclipse::Graphics
{
    class RENDERER_API IGraphicsBuffer
    {
    public:
        IGraphicsBuffer() = default;
        virtual ~IGraphicsBuffer() = default;

        template <class T>
        void GetBuffer(T*& aBuffer);
        
        template <class T>
        void SetOrCreateBuffer(int aBufferIndex, T& aBuffer);

        template <class T>
        void SetOrCreateBuffer(int aBufferIndex, T* aBuffer);

        template <class T>
        void SetOrCreateBuffer(int aBufferIndex);

    private:
        virtual void AddInternalBuffer(std::type_index aTypeInfo, size_t bufferSize) = 0;
        virtual void SetInternal(int aBufferIndex, std::type_index aTypeInfo, void* aBuffer, size_t bufferSize) = 0;

    private:
        std::unordered_map<std::type_index, void*> myBufferStorage;
    };

    template <class T>
    void IGraphicsBuffer::GetBuffer(T*& aBuffer)
    {
        if (!myBufferStorage.contains(typeid(T)))
        {
            myBufferStorage[typeid(T)] = new T();
            AddInternalBuffer(typeid(T), sizeof(T));
        }
        
        aBuffer = static_cast<T*>(myBufferStorage[typeid(T)]);
    }

    template <class T>
    void IGraphicsBuffer::SetOrCreateBuffer(int aBufferIndex, T& aBuffer)
    {
        if (!myBufferStorage.contains(typeid(T)))
        {
            myBufferStorage[typeid(T)] = new T();
            AddInternalBuffer(typeid(T), sizeof(T));
        }

        void*& buffer = myBufferStorage[typeid(T)];
        memcpy(buffer, &aBuffer, sizeof(T));

        SetInternal(aBufferIndex, typeid(T), buffer, sizeof(T));
    }

    template <class T>
    void IGraphicsBuffer::SetOrCreateBuffer(int aBufferIndex, T* aBuffer)
    {
        if (!myBufferStorage.contains(typeid(T)))
        {
            myBufferStorage[typeid(T)] = new T();
            AddInternalBuffer(typeid(T), sizeof(T));
        }

        void*& buffer = myBufferStorage[typeid(T)];
        memcpy(buffer, aBuffer, sizeof(T));

        SetInternal(aBufferIndex, typeid(T), buffer, sizeof(T));
    }

    template <class T>
    void IGraphicsBuffer::SetOrCreateBuffer(int aBufferIndex)
    {
        if (!myBufferStorage.contains(typeid(T)))
        {
            myBufferStorage[typeid(T)] = new T();
            AddInternalBuffer(typeid(T), sizeof(T));
        }

        SetInternal(aBufferIndex, typeid(T), myBufferStorage[typeid(T)], sizeof(T));
    }
}
