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
#include "EclipsedEngine/Reflection/Reflection_Macros.h"
#include "EclipsedEngine/Replication/Replication_Macros.h"
namespace Eclipse
{
    class Sprite;
    class Shader;

    class Transform2D;
    
    class ECL_API SpriteRenderer2D : public BaseRenderComponent
    {
        MAKE_REFLECTABLE(SpriteRenderer2D);
        REFLECTION_FLAG(COMPONENT);

        COMPONENT_BASE_2(SpriteRenderer2D, 20)

    public:
        void OnComponentAdded() override;

        void Render() override;
        void Draw(unsigned aProgramID = 0);


        void SetSprite(const Assets::GUID& aGuid);
        void SetSprite(const Assets::Texture& aSprite);
        [[nodiscard]] Assets::Texture GetSprite() const { return mySprite; }

        void SetMaterial(const Assets::GUID& aGuid);
        void SetMaterial(const Assets::Material& aMaterial);
        [[nodiscard]] Assets::Material GetMaterial() const { return myMaterial; }

        void SetColor(const Math::Color& aColor);
        [[nodiscard]] Math::Color GetColor() const { return myColor; }

        void SetSpriteRect(const Math::Vector2f& aMin, const Math::Vector2f& aMax);

        void SetXMirror(bool aMirror) { myMirror.x = aMirror; }
        void SetYMirror(bool aMirror) { myMirror.y = aMirror; }

        float GetZIndex() override { return zIndex; }


        void OnChangeColor();
        void OnChangeMaterial();
        void OnChangeSprite();
        void OnChangeMirror();
        void OnChangeZIndex();

        Math::Vector2f spriteRectMin = { 0.f, 0.f };
        Math::Vector2f spriteRectMax = { 1.f, 1.f };

    private:
        bool hasSprite = false;
        bool hasMaterial = false;

        SpriteBuffer mySpriteBuffer;
        
        Assets::Material PixelPickMaterial;

        Math::Vector2f Bounds;

    private:
        [[SerializeField, OnChange(OnChangeSprite)]]
        Assets::Texture mySprite;

        [[SerializeField, OnChange(OnChangeMaterial)]]
        Assets::Material myMaterial;

        [[SerializeField, OnChange(OnChangeColor)]]
        Math::Color myColor;

        [[SerializeField, OnChange(OnChangeMirror)]]
        Math::Vector2<bool> myMirror = { false, false };
        
        [[SerializeField, OnChange(OnChangeZIndex)]]
        int zIndex = 0;
    };
}
