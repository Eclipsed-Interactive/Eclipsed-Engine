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
        void OnComponentAdded() override;
        void TransformUpdate();
        
        void Render() override;
        void Draw();

        bool hasSprite = false;
        bool hasMaterial = false;

        MANUAL_REPLICATED_SERIALIZED_FIELD(Texture, sprite, UIImage);
        SERIALIZED_FIELD(Material, material);

        TransformBuffer myTransformBuffer;
    };
}