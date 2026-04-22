#ifdef ECLIPSED_EDITOR
#pragma once

#include "EclipsedEngine/Editor/EditorContext.h"
#include "CoreEngine/Macros/defines.h"

namespace Eclipse::Editor
{
    class ECLIPSED_API EditorApplication
    {
    public:
        void Init();
        int Update();

        void Shutdown();

    private:
        EditorContexts myContext;
    };
}
#endif