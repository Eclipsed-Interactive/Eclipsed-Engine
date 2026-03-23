#pragma once

#include "EntityEngine/ComponentManager.h"
#include "EclipsedEngine/Components/Component.h"
#include "CoreEngine/GraphicsBuffers/CanvasBuffer.h"

namespace Eclipse
{
    class Canvas : public Component
    {
        COMPONENT_BASE_2(Canvas, 999999);

    public:

        void SetCanvasTransformProperties();
        void EditorUpdate() override;
        void OnComponentAdded() override;

        void TransformUpdate();

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
