#pragma once

#include "ECS/ComponentManager.h"
#include "EclipsedEngine/Components/Component.h"
#include "Core/GraphicsBuffers/CanvasBuffer.h"
#include "EclipsedEngine/Reflection/Reflection_Macros.h"
#include "EclipsedEngine/Replication/Replication_Macros.h"
#include <vector>

namespace Eclipse
{
    class ECL_API Canvas : public Component
    {
        COMPONENT_BASE_2(Canvas, 999999);

    public:

        void SetCanvasTransformProperties();
        void EditorUpdate() override;
        void OnComponentAdded() override;

        void TransformUpdate();

        void SetCanvasForChildren(std::vector<GameObject*>& Children);

        SERIALIZED_FIELD_DEFAULT(Math::Vector2<float>, ReferenceResolution, Math::Vector2f(1920, 1080));
        SERIALIZED_FIELD_DEFAULT(bool, WorldSpace, false);

        struct EditorCanvasCameraTransform
        {
            Math::Vector2f PositionOffset = { 0, 0 };
            float Rotation = 0.f;
            Math::Vector2f ScaleMultiplier = { 1, 1 };
        } canvasCameraTransform;

        static inline bool drawCanvasGizmos = true;

        CanvasBuffer myCanvasBuffer;
    };
}
