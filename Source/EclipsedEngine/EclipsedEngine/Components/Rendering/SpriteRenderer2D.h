#pragma once

#include "EclipsedEngine/Components/Rendering/BaseRenderComponent.h"
#include "EclipsedEngine/Components/Component.h"

#include <string>
#include "ECS/ComponentManager.h"

#include "Core/Math/Vector/Vector2.h"
#include "Core/Math/Color.h"

#include "Assets/AssetManager.h"
#include "EclipsedEngine.Core.hpp"
#include "Core/GraphicsBuffers/SpriteBuffer.h"
#include "Core/GraphicsBuffers/TransformBuffer.h"

namespace Eclipse
{
    class Sprite;
    class Shader;

    class Transform2D;
    
    class ECL_API SpriteRenderer2D : public BaseRenderComponent
    {
        COMPONENT_BASE_2(SpriteRenderer2D, 20)

    public:
        void OnComponentAdded() override;

        void Render() override;
        void Draw(unsigned aProgramID = 0);

        Assets::Texture GetSprite();

        void SetSprite(const Assets::GUID& aGuid);
        void SetSprite(const Assets::Texture& aSprite);

        void SetMaterial(const Assets::GUID& aGuid);
        void SetMaterial(const Assets::Material& aMaterial);
        Assets::Material* GetMaterial() { return nullptr; }

        void SetSpriteRect(const Math::Vector2f& aMin, const Math::Vector2f& aMax);

        void SetXMirror(bool aMirror);/* { mirroredX = aMirror; }*/
        void SetYMirror(bool aMirror); /*{ mirroredY = aMirror; }*/

        //void DrawInspector() override;

        Math::Vector2f spriteRectMin = { 0.f, 0.f };
        Math::Vector2f spriteRectMax = { 1.f, 1.f };

    private:
        bool hasSprite = false;
        bool hasMaterial = false;

        bool mirroredX = false;
        bool mirroredY = false;

        TransformBuffer myTransformBuffer;
        SpriteBuffer mySpriteBuffer;
        
        MANUAL_REPLICATED_SERIALIZED_FIELD(Assets::Texture, sprite, SpriteRenderer2D);
        SERIALIZED_FIELD(Assets::Material, material);

        Assets::Material PixelPickMaterial;

        Math::Vector2f Bounds;
    };
}
