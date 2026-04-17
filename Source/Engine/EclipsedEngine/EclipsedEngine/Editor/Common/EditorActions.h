#ifdef ECLIPSED_EDITOR
#pragma once

#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"

#include <vector>

namespace Eclipse
{
    class GameObject;

    namespace Editor
    {
        class EditorActions
        {
        public:
            enum Action
            {
                A_NONE,

                A_Save,

                A_Copy,
                A_Paste,
                A_Duplicate,

                A_Undo,
                A_Redo
            };

        public:
            EditorActions() = default;
            ~EditorActions() = default;

            static void Update();

            static void SaveScene();
            static void Save();

            static void CopyGameObject(unsigned activeGO, rapidjson::Value& gameobjectJson, rapidjson::Document::AllocatorType& anAllocator);
            static rapidjson::StringBuffer CopyObject(int aObjectID, bool aCopyToClipboard);
            static void Copy();

            static void Paste();

            static void ReactToAction(Action anAction);

        };
    }
}
#endif