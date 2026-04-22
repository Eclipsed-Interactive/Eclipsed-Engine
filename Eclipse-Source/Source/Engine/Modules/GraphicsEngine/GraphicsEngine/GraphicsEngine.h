#pragma once

#include "CoreEngine/ErrorCodes.h"

namespace Eclipse
{
    class BaseGraphicsBuffer;
    class OpenGLGraphicsBuffer;

    class GraphicsEngine
    {
    public:
        GraphicsEngine() = default;
        virtual ~GraphicsEngine() = default;

        template <class T>
        static void InitSpecifiedAPI();

        template<class T>
        static T* Get() { return reinterpret_cast<T*>(Instance); }
        
        virtual ErrorCode Init() = 0;

        BaseGraphicsBuffer* GetGraphicsBuffer() { return myGraphicsBuffer; }

        
    public:
        enum class MouseCursor {
            Hand,
            Grab
        };
        
        BaseGraphicsBuffer* myGraphicsBuffer;
        
        static inline GraphicsEngine* Instance;
    };

    template <class T>
    void GraphicsEngine::InitSpecifiedAPI()
    {
        Instance = new T();
    }
}
