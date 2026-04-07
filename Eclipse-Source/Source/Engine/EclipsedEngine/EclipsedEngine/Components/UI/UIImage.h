#pragma once

#include "Components/Rendering/BaseRenderComponent.h"
#include "EclipsedEngine/Components/Component.h"

#include "AssetEngine/Assets/Material.h"
#include "AssetEngine/Assets/Texture.h"

namespace Eclipse
{
    class UIImage : public BaseRenderComponent
    {
        COMPONENT_BASE_2(UIImage, 0)

    public:
        void EditorUpdate() override;
        void OnComponentAdded() override;
        void TransformUpdate();
        
        void Render() override;
        void Draw();

        bool hasSprite = false;
        bool hasMaterial = false;

        Math::Vector2f spriteRectMin = { 0.f, 0.f };
        Math::Vector2f spriteRectMax = { 1.f, 1.f };

        MANUAL_REPLICATED_SERIALIZED_FIELD(Texture, sprite, UIImage);
        SERIALIZED_FIELD(Material, material);

        TransformBuffer myTransformBuffer;
    };
}