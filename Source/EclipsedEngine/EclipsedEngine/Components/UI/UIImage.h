#pragma once

#include "EclipsedEngine/Components/Rendering/BaseRenderComponent.h"
#include "EclipsedEngine/Components/Component.h"

#include "AssetSystem/Assets/Types/Material.h"
#include "AssetSystem/Assets/Types/Texture.h"
#include <Core/GraphicsBuffers/TransformBuffer.h>

#include "EclipsedEngine.Core.hpp"

namespace Eclipse
{
    class ECL_API UIImage : public BaseRenderComponent
    {
        MAKE_REFLECTABLE(UIImage);
        REFLECTION_FLAG(COMPONENT);

        COMPONENT_BASE_2(UIImage, 0)

    public:
#ifdef ECL_EDITOR
        void EditorUpdate() override;
#endif // ECL_EDITOR
        void OnComponentAdded() override;
        void TransformUpdate();
        
        void Render() override;
        void Draw();

        bool hasSprite = false;
        bool hasMaterial = false;

        Math::Vector2f spriteRectMin = { 0.f, 0.f };
        Math::Vector2f spriteRectMax = { 1.f, 1.f };

        [[SerializeField]]
        Texture sprite;

        [[SerializeField]]
        Material material;

        TransformBuffer myTransformBuffer;
    };
}